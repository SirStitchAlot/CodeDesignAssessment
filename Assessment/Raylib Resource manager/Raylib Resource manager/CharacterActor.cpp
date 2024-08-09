/** CharacterActor.cpp
 * 
 * See header file for details.
 */

#include "CharacterActor.h"

#include <cstdio>

#define MOVING_FRICTION 0.2f
#define STANDING_FRICTION 1.0f

CharacterActor::CharacterActor(float widthScale) : 
		groundSensor(NULL), torsoPhysicsObject(this), legsFrictionAdjuster(this) {
	init(widthScale);
}

CharacterActor::~CharacterActor() {
}

void CharacterActor::createPhysicsBody(b2World &world, float worldScale, 
		const b2Vec2 &position, const b2Vec2 &velocity) {
	raylib::Rectangle boundingBox = getBoundingBox(worldScale);
	
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	bodyDef.position = position;
	bodyDef.linearVelocity = velocity;
	
	float halfWidth = boundingBox.width * widthScale / 2.0f;
	float halfHeight = boundingBox.height / 2.0f;
	
	// Need to tweak the friction depending on the situation
	legsFrictionAdjuster.setPreSolveFunc([this](PhysicsObject *thisObject, PhysicsObject *otherObject,
	b2Contact *contact, const b2Manifold *oldManifold) {
		// Increase the friction so that the character can stand still on slopes, but still 
		// walk them
		float otherFriction = friction;
		thisObject->getOurFixture(contact)->SetFriction(friction);
		b2Fixture *otherFixture = thisObject->getOtherFixture(contact);
		if(otherFixture) { otherFriction = otherFixture->GetFriction(); }
		float adjustedFriction = sqrtf(friction * otherFriction);
		contact->SetFriction(adjustedFriction);
	});
	
	// Approximating our character with a rectangle and a circle for feet.
	// The circle avoids the character catching at the vertices at tile edges.
	// See the following for a description of the problem: https://www.iforce2d.net/b2dtut/ghost-vertices
	physicsBody = world.CreateBody(&bodyDef);
	
	float torsoHeight = boundingBox.height - halfWidth;
	float torsoHalfHeight = torsoHeight / 2.0f;
	
	b2PolygonShape torsoShape;
	b2Vec2 boxCentre(0.0f, -boundingBox.height + torsoHalfHeight);
	torsoShape.SetAsBox(halfWidth, torsoHalfHeight, boxCentre, 0.0f);
	b2FixtureDef torsoDef;
	torsoDef.shape = &torsoShape;
	torsoDef.density = 1.0f;
	torsoPhysicsObject.attachToFixture(torsoDef);
	physicsBody->CreateFixture(&torsoDef);
	
	
	b2CircleShape legsShape;
	// Make the radius slightly bigger, so the edge of the torso's rectangle can't accidentally act as a hook
	legsShape.m_radius = halfWidth + 1.0f / worldScale; 
	legsShape.m_p.y = -halfWidth;
	b2FixtureDef legsDef;
	legsDef.shape = &legsShape;
	legsDef.density = 1.0f;
	legsFrictionAdjuster.attachToFixture(legsDef); 
	physicsBody->CreateFixture(&legsDef);
	
	// Adding a ground sensor, for reliable "am I on the ground" detection
	groundSensor.setContactHandler([this](PhysicsObject *thisObject, PhysicsObject *otherObject,
			b2Contact *contact, bool contactBegin) {
		groundContactCount += contactBegin ? 1 : -1;
		isOnGround = (groundContactCount != 0);
	});
	
	// The foot sensor should stick out by a few pixels so slight floating-point inaccuracies don't trigger
	// repeated on/off the ground events when the character is actually walking
	float footSensorOffset = 3.0f / worldScale; 
	b2CircleShape footSensor;
	footSensor.m_radius = halfWidth;
	footSensor.m_p.y = legsShape.m_p.y + footSensorOffset;
	b2FixtureDef footSensorDef;
	footSensorDef.shape = &footSensor;
	footSensorDef.isSensor = true;
	groundSensor.attachToFixture(footSensorDef);
	physicsBody->CreateFixture(&footSensorDef);
}

void CharacterActor::goUp() {
	if(isOnGround) {
		b2Vec2 velocity = getVelocity();
		velocity.y = -jumpSpeed;
		setVelocity(velocity);
	}
}
	
void CharacterActor::goLeft() {
	if(isOnGround) {
		b2Vec2 velocity = getVelocity();
		velocity.x = -walkSpeed;
		setVelocity(velocity);
		facingRight = false;
	}
	
	// For fast motion
	// NOTE: Put here so it also works after landing
	setFriction(MOVING_FRICTION);
}
	
void CharacterActor::goRight() {
	if(isOnGround) {
		b2Vec2 velocity = getVelocity();
		velocity.x = walkSpeed;
		setVelocity(velocity);
		facingRight = true;
	}

	// For fast motion
	// NOTE: Put here so it also works after landing
	setFriction(MOVING_FRICTION);
}

void CharacterActor::goNowhere() {
	if(isOnGround) {
		b2Vec2 velocity = getVelocity();
		velocity.x = 0;
		setVelocity(velocity);		
	}
	
	// So we can stand on slopes
	// NOTE: Put here so it also works after landing
	setFriction(STANDING_FRICTION);
}
	
void CharacterActor::goDown() {
}
	
void CharacterActor::doInteractWith() {
}

void CharacterActor::setFriction(float friction) {
	this->friction = friction;
}

void CharacterActor::init(float widthScale) {
	facingRight = true;
	groundContactCount = 0;
	setFriction(MOVING_FRICTION);
	
	walkSpeed = 12;
	jumpSpeed = 2.2f * walkSpeed;
	
	this->widthScale = widthScale;
}