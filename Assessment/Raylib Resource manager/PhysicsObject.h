/** PhysicsObject.h
 *
 * Base class for anything that can collide in the world.
 *
 * @author Hans de Ruiter
 *
 * @copyright (c) 2022 by Kea Sigma Delta Limited, all rights reserved
 *
 * See License.txt for license.
 */

#pragma once

#include "box2d/box2d.h"

#include <functional>

class PhysicsObject;
class Actor;

/** Defines the callback for physics contacts.
 *
 * WARNINGS: Do NOT create or destroy physics entities within this method! 
 *
 * @param thisObject an object
 * @param otherObject the object that thisObject collided with, or is touching
 * WARNING: This pointer may be NULL if no PhysicsObject was created for the other object
 * @param contact the contact object. This is useful so that we can override things such as the 
 * coefficient of friction
 * @param contactBegin set to true if the two objects started touching, and false if the touch ended
 */
typedef std::function<void (PhysicsObject *thisObject, PhysicsObject *otherObject,
	b2Contact *contact, bool contactBegin)> POContactHandler;
	
/** Defines the callback to perform pre-solve operations on a physics contact event.
 *
 * WARNINGS: Do NOT create or destroy physics entities within this method! 
 *
 * @param thisObject an object
 * @param otherObject the object that thisObject collided with, or is touching
 * WARNING: This pointer may be NULL if no PhysicsObject was created for the other object
 * @param contact the contact object. This is useful so that we can override things such as the 
 * coefficient of friction
 * @param oldManifold the contact's old manifold
 */
typedef std::function<void (PhysicsObject *thisObject, PhysicsObject *otherObject,
	b2Contact *contact, const b2Manifold *oldManifold)> POPreSolveFunc;

/** Defines the callback to perform post-solve operations on a physics contact event.
 *
 * WARNINGS: Do NOT create or destroy physics entities within this method! 
 *
 * @param thisObject an object
 * @param otherObject the object that thisObject collided with, or is touching
 * WARNING: This pointer may be NULL if no PhysicsObject was created for the other object
 * @param contact the contact object. This is useful so that we can override things such as the 
 * coefficient of friction
 * @param oldManifold the contact's old manifold
 */
typedef std::function<void (PhysicsObject *thisObject, PhysicsObject *otherObject,
	b2Contact *contact, const b2ContactImpulse *impulse)> POPostSolveFunc;

/** Base class for physics objects involved in collisions.
 * Custom behaviour can be implemented via the beginContact() & endContact() methods. Or, they can
 * be added to the PhysicsCollisionHandler's collision actions.
 *
 * HOW TO USE:
 * - Make your object a child-class of PhysicsObject (e.g., Actor is a child-class)
 *   NOTE: You can create separate PhysicsObjects for sub-components or physics sensors (objects
 *   that detect contact but don't interact physically)
 * - Create the physics shapes using box2d's API as normal
 * - Call attachToFixture() to attach it to this object's box2d fixture (can attach to multiple
 *   fixtures)
 * - To respond to physics contact/collision events, you can do one of the following:
 *     - Override the beginContact()/endContact methods (for child-classes)
 *     - Call setContactHandler to add a custom callback 
 *       NOTE: Called by beginContact()/endContact(), unless you override them in a child-class
 *
 * PERFORMANCE SUGGESTIONS:
 * When handling interactions between objects in the world:
 * - Keep class hierarchies shallow, and as few as possible
 * - Prefer checking the typeid over dynamic_cast
 * - Consider using std::variant<A*, B*, ...etc,> and std::visit on child-classes of PhysicsObject:
 *   https://dzone.com/articles/double-dispatch-in-c
 */
class PhysicsObject {
public:
	/** Create a new physics object.
	 * 
	 * @param actor the actor  this physics object belongs to (if any)
	 * NOTE: We recommend setting this to NULL for sensor objects, otherwise the sensor may trigger
	 * unwanted actions (e.g., die() is called when the physical object itself was not yet touched)
	 */
	PhysicsObject(Actor *actor);
	
	virtual ~PhysicsObject();

	/** Set this as b2Fixture's physics object. 
	 * This enables handling via PhysicsContactHandler.
	 */
	void attachToFixture(b2FixtureDef &fixtureDef);

	/** Gets which actor this physics object belongs to (if any).
	 */
	Actor* getActor();
	
	/** Sets the (optional) contact handler callback for this object.
	 * This enables custom object behaviour. For example, a contact sensor could tell if a character
	 * is on the ground.
	 *
	 * WARNING: Do NOT create or destroy physics entities within the callback! 
	 */
	void setContactHandler(POContactHandler handler);
	
	/** Sets the (optional) pre-solve callback for this object.
	 * This allows you to inspect the contact pre-solving, and perform modifications (e.g., changing
	 * the friction coefficient).
	 */
	void setPreSolveFunc(POPreSolveFunc func);
	
	/** Sets the (optional) post-solve callback for this object.
	 * This allows you to inspect the contact fter the solver has completed.
	 */
	void setPostSolveFunc(POPostSolveFunc func);
	
	/** Gets this object's b2Fixture from the b2Contact object.
	 */
	b2Fixture* getOurFixture(b2Contact *contact);
	
	/** Gets the b2Fixture belonging to the other object involved in the physics ontact.
	 */
	b2Fixture* getOtherFixture(b2Contact *contact);
	
	/** Returns true if the given fixture belongs to this object.
	 */
	bool isOurFixture(b2Fixture *fixture);
	
	/** Called when this object starts touching another.
	 *
	 * WARNING: Do NOT create or destroy physics entities within this method! 
	 *
	 * @param otherObject the object that this one has started touching
	 * @param contact the underlying contact object
	 */
	virtual void beginContact(PhysicsObject *otherObject, b2Contact *contact);
	
	/** Called when this object stops touching another.
	 *
	 * WARNING: Do NOT create or destroy physics entities within this method! 
	 *
	 * @param otherObject the object that this one has stopped touching
	 * @param contact the underlying contact object
	 */
	virtual void endContact(PhysicsObject *otherObject, b2Contact *contact);
	
	/** This is called after a contact is updated. 
	 * This allows you to inspect a contact before it goes to the solver, and make modifications
	 * (e.g., change the friction).
	 *
	 * @param otherObject the object that this one is touching
	 * @param contact the underlying contact object
	 * @param oldManifold the old contact manifold
	 */
	virtual void preSolve(PhysicsObject *otherObject, b2Contact *contact, const b2Manifold *oldManifold);
	
	/** This lets you inspect a contact after the solver is finished. 
	  This is useful for inspecting impulses. 
	 *
	 * @param otherObject the object that this one is touching
	 * @param contact the underlying contact object
	 * @param impulse the contact's impulse
	 */
	virtual void postSolve(PhysicsObject *otherObject, b2Contact *contact, const b2ContactImpulse *impulse);
	
protected:
	POContactHandler contactHandler;
	POPreSolveFunc preSolveFunc;
	POPostSolveFunc postSolveFunc;
	
	Actor *actor;
};
