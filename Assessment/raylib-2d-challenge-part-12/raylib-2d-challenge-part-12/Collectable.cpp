/** Collectable.cpp
 * 
 * See header file for details.
 */

#include "Collectable.h"

#include "ResourceManager.h"

#include <algorithm>

// ----- PRIVATE FUNCTIONS/DEFINITIONS -----

void collectableTouched(PhysicsObject *thisObject, PhysicsObject *otherObject, 
        b2Contact *contact, bool contactBegin) {
    if(!otherObject) {
        // The other object cannot interact with this object
        return;
    }
    Collectable *thisCollectable = static_cast<Collectable *>(thisObject->getActor());
    Actor *otherActor = otherObject->getActor();
    if(!otherActor) {
        // No actor touched this object
        return;
    }

    // Tell the other object to collect this (if it can)
    otherActor->collect(thisCollectable);
}


// ----- PUBLIC METHODS/FIELDS -----

Collectable::Collectable(const char *typeName, const char *atlasFileName,   
        const char *collectSoundFileName, int value) : physicsObject(this) {
    state = CS_AVAILABLE;

    textureAtlas = std::make_shared<TextureAtlas>(atlasFileName);

    sprite = textureAtlas->getSprite(typeName);
    if(!sprite) {
        throw std::runtime_error(TextFormat("Couldn't find sprite %s in texture atlas %s.", typeName, atlasFileName));
    }

    auto *defaultAnim = sprite->getAnimation("default");
    if(!defaultAnim) {
        throw std::runtime_error(TextFormat("Texture atlas %s doesn't have a \"default\" animation for sprite %s.", atlasFileName, typeName));
    }
    defaultAnimPlayer.reset(new TASpriteAnimationPlayer(textureAtlas, defaultAnim));

    auto *collectAnim = sprite->getAnimation("collect");
    if(collectAnim) {
        collectAnimPlayer.reset(new TASpriteAnimationPlayer(textureAtlas, collectAnim));
    }

    if(collectSoundFileName) {
        collectSound = SoundManager::load(collectSoundFileName);
    }

    itemValue = value;
}

Collectable::~Collectable() {
}

int Collectable::collect() {
    if(state != CS_AVAILABLE) {
        // This item has already been collected
        return 0;
    }

    state = CS_COLLECTING;

    if(collectSound) {
        collectSound->Play();
    }

    return itemValue;
}

bool Collectable::update(float elapsedTime) {
    switch(state) {
    case CS_AVAILABLE:
        defaultAnimPlayer->update(elapsedTime);
        break;
    case CS_COLLECTING:
        if(!collectAnimPlayer || !collectAnimPlayer->update(elapsedTime)) {
            // Animation either finished, or non-existent
            physicsBody->SetEnabled(false);
            state = CS_COLLECTED;
        }
        break;
    case CS_COLLECTED:
    default:
        // Collected. No longer exists in the world (outside of the player).
        die();
    }
	return isAlive;
}

void Collectable::draw(float worldScale) {
    auto &b2Pos = getPosition();
    raylib::Vector2 position(b2Pos.x, b2Pos.y);
    switch(state) {
    case CS_AVAILABLE:
        defaultAnimPlayer->draw(position, worldScale);
        break;
    case CS_COLLECTING:
        if(collectAnimPlayer) {
            collectAnimPlayer->draw(position, worldScale);
        }
        break;
    case CS_COLLECTED:
    default:
        // Collected. No longer exists in the world (outside of the player).
        die();
    }
}

void Collectable::createPhysicsBody(b2World &world, float worldScale, 
		const b2Vec2 &position, const b2Vec2 &velocity) {
    const raylib::Vector2 &size = sprite->size / worldScale;

    b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	bodyDef.position = position;
	bodyDef.linearVelocity = velocity;
    physicsBody = world.CreateBody(&bodyDef);

    // Treating the collectable as circle
    float halfWidth = size.x / 2.0f;
	float halfHeight = size.y / 2.0f;
    float radius = std::min(halfWidth, halfHeight);

    b2CircleShape shape;
	shape.m_radius = radius;
	b2FixtureDef shapeDef;
	shapeDef.shape = &shape;
	shapeDef.density = 1.0f;
    physicsObject.attachToFixture(shapeDef);
	physicsBody->CreateFixture(&shapeDef);
    physicsObject.setContactHandler(collectableTouched);
}

void Collectable::die() {
    if(!collectSound || !collectSound->IsPlaying()) {
        // The collect sound is done, so we can disappear now.
        Actor::die();
    }

}