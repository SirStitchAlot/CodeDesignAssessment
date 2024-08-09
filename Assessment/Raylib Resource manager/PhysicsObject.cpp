/** PhysicsObject.cpp
 *
 * See header file for details.
 */

#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(Actor *actor) : actor(actor) {
}
	
PhysicsObject::~PhysicsObject() {
}

void PhysicsObject::attachToFixture(b2FixtureDef &fixtureDef) {
	fixtureDef.userData.pointer = (uintptr_t)this;
}

Actor* PhysicsObject::getActor() {
	return actor;
}
	
void PhysicsObject::setContactHandler(POContactHandler handler) {
	this->contactHandler = handler;
}

void PhysicsObject::setPreSolveFunc(POPreSolveFunc func) {
	this->preSolveFunc = func;
}
	
void PhysicsObject::setPostSolveFunc(POPostSolveFunc func) {
	this->postSolveFunc = func;
}

b2Fixture* PhysicsObject::getOurFixture(b2Contact *contact) {
	b2Fixture *fixture = contact->GetFixtureA();
	if(isOurFixture(fixture)) {
		return fixture;
	}
	fixture = contact->GetFixtureB();
	if(isOurFixture(fixture)) {
		return fixture;
	}
	
	return NULL;
}

b2Fixture* PhysicsObject::getOtherFixture(b2Contact *contact) {
	b2Fixture *fixture = contact->GetFixtureA();
	if(!isOurFixture(fixture)) {
		return fixture;
	}
	fixture = contact->GetFixtureB();
	if(!isOurFixture(fixture)) {
		return fixture;
	}
	
	return NULL;
}

bool PhysicsObject::isOurFixture(b2Fixture *fixture) {
	return fixture->GetUserData().pointer == (uintptr_t)this;
}
	
void PhysicsObject::beginContact(PhysicsObject *otherObject, b2Contact *contact) {
	if(contactHandler) {
		contactHandler(this, otherObject, contact, true);
	}
}
	
void PhysicsObject::endContact(PhysicsObject *otherObject, b2Contact *contact) {
	if(contactHandler) {
		contactHandler(this, otherObject, contact, false);
	}
}

void PhysicsObject::preSolve(
		PhysicsObject *otherObject, b2Contact *contact, const b2Manifold *oldManifold) {
	if(preSolveFunc) {
		preSolveFunc(this, otherObject, contact, oldManifold);
	}
}
	
void PhysicsObject::postSolve(PhysicsObject *otherObject, b2Contact *contact, const b2ContactImpulse *impulse) {
	if(postSolveFunc) {
		postSolveFunc(this, otherObject, contact, impulse);
	}
}
