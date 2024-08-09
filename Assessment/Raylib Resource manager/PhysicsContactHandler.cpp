/**PhysicsContactHandler.cpp
 *
 * See header file for details.
 */

#include "PhysicsContactHandler.h"

#include "PhysicsObject.h"

PhysicsContactHandler::PhysicsContactHandler() {
}

PhysicsContactHandler::~PhysicsContactHandler() {
}

void PhysicsContactHandler::BeginContact(b2Contact *contact) {
	PhysicsObject *objectA = (PhysicsObject*)contact->GetFixtureA()->GetUserData().pointer;
	PhysicsObject *objectB = (PhysicsObject*)contact->GetFixtureB()->GetUserData().pointer;
	if(objectA) {
		objectA->beginContact(objectB, contact);
	}
	if(objectB) {
		objectB->beginContact(objectA, contact);
	}
}
	
void PhysicsContactHandler::EndContact(b2Contact *contact) {
	PhysicsObject *objectA = (PhysicsObject*)contact->GetFixtureA()->GetUserData().pointer;
	PhysicsObject *objectB = (PhysicsObject*)contact->GetFixtureB()->GetUserData().pointer;
	if(objectA) {
		objectA->endContact(objectB, contact);
	}
	if(objectB) {
		objectB->endContact(objectA, contact);
	}
}

void PhysicsContactHandler::PreSolve(b2Contact *contact, const b2Manifold *oldManifold) {
	PhysicsObject *objectA = (PhysicsObject*)contact->GetFixtureA()->GetUserData().pointer;
	PhysicsObject *objectB = (PhysicsObject*)contact->GetFixtureB()->GetUserData().pointer;
	if(objectA) {
		objectA->preSolve(objectB, contact, oldManifold);
	}
	if(objectB) {
		objectB->preSolve(objectA, contact, oldManifold);
	}
}
	
void PhysicsContactHandler::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) {
	PhysicsObject *objectA = (PhysicsObject*)contact->GetFixtureA()->GetUserData().pointer;
	PhysicsObject *objectB = (PhysicsObject*)contact->GetFixtureB()->GetUserData().pointer;
	if(objectA) {
		objectA->postSolve(objectB, contact, impulse);
	}
	if(objectB) {
		objectB->postSolve(objectA, contact, impulse);
	}
}