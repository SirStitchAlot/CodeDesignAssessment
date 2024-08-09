/** Actor.cpp
 *
 * See the header file for details.
 */

#include "Actor.h"

#include "Collectable.h"
#include "Functions.hpp"
#include "Text.hpp"

#include <cfloat>

static const b2Vec2 zeroVec(0.0f, 0.0f);

Actor::Actor() {
	isAlive = true;
	physicsBody = NULL;
	isOnGround = false;
	wasOnGround = false;
	walletValue = ACTOR_NO_WALLET;
}
	
Actor::~Actor() {
	deletePhysicsBody();
}

bool Actor::update(float elapsedTime) {
	return isAlive;
}

void Actor::draw(float worldScale) {
	raylib::Text message("Override Actor::draw()", 20, RED, ::GetFontDefault(), 1.0f);
	auto textSize = message.MeasureEx();
	const auto &position = getPosition();
	message.Draw(raylib::Vector2(position.x, position.y), 0, textSize / 2);
}

bool Actor::collect(Collectable *collectable) {
	if(walletValue != ACTOR_NO_WALLET) {
		walletValue += collectable->collect();
		return true;
	} 
	
	return false;
}

int Actor::getWalletValue() const {
	return walletValue;
}

void Actor::drawBoundingBox(const Color &color, float worldScale) {
	auto boundingBox = getBoundingBox(worldScale);
	
	float lineThickness =2.0f / worldScale;
	DrawRectangleLinesEx(boundingBox, lineThickness, color);
}

raylib::Rectangle Actor::getBoundingBox(float worldScale) {
	const auto &position = getPosition();
	return raylib::Rectangle(position.x, position.y);
}

const b2Vec2& Actor::getPosition() const {
	if(physicsBody) {
		return physicsBody->GetPosition();
	} else {
		return zeroVec;
	}
}
	
void Actor::setPosition(const b2Vec2 &position) {
	if(physicsBody) {
		return physicsBody->SetTransform(position, 0);
	} else {
		throw std::runtime_error("Can't set actor's position, because it has no physics body.");
	}
}
	
const b2Vec2& Actor::getVelocity() const {
	if(physicsBody) {
		return physicsBody->GetLinearVelocity();
	} else {
		return zeroVec;
	}
}
	
void Actor::setVelocity(const b2Vec2 &velocity) {
	if(physicsBody) {
		return physicsBody->SetLinearVelocity(velocity);
	} else {
		throw std::runtime_error("Can't set actor's velocity, because it has no physics body.");
	}
}


bool Actor::isMoving() const {
	return getVelocity().LengthSquared() > 10.0f * FLT_EPSILON;
}

void Actor::die() {
	isAlive = false;
}

void Actor::createPhysicsBody(b2World &world, float worldScale, 
		const b2Vec2 &position, const b2Vec2 &velocity) {
}

void Actor::deletePhysicsBody() {
	if(physicsBody) {
		physicsBody->GetWorld()->DestroyBody(physicsBody);
		physicsBody = NULL;
	}
}
