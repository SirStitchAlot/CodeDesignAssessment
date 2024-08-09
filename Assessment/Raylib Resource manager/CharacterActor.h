/** CharacterActor.h
 *
 * Base class for characters (i.e., a person or animal with arms and legs).
 *
 * @author Hans de Ruiter
 *
 * @copyright (c) 2022 by Kea Sigma Delta Limited, all rights reserved
 *
 * See License.txt for license.
 */

#pragma once

#include "Actor.h"

#include "PhysicsObject.h"

/** Base class for simple characters (i.e., a person or animal with arms and legs).
 * This will create and manage the physics object for the character.
 *
 * The physics shape is based on the bounding box returned from getBoundingBox()
 * returned by the child-class.
 */
class CharacterActor : public Actor {
public:
	/** Creates a new character actor.
	 *
	 * @param widthScale the physics shape's width is this character's bounding box width times this value.
	 * This 
	 */
	CharacterActor(float widthScale = 0.80);
	
	virtual ~CharacterActor();

	void createPhysicsBody(b2World &world, float worldScale, 
		const b2Vec2 &position, const b2Vec2 &velocity) override;
	
	void goUp() override;
	
	void goLeft() override;
	
	void goRight() override;
	
	void goDown() override;
	
	void goNowhere() override;
	
	/** Select, push, pull, punch, fire, etc.
	 */
	void doInteractWith() override;

protected:
	/** Sets the character's friction with objects around it (like the ground).
	 */
	void setFriction(float friction);

	/** Indicates if the character is facing left or right.
	 */
	bool facingRight;
	
	// Parameters that control character's motion
	float jumpSpeed;
	float walkSpeed;
	
	// Physics shape parameters
	float widthScale;
	
	/** The "are we on the ground" sensor.
	 */
	PhysicsObject groundSensor;
	int groundContactCount;
	
	/** Need to tweak friction depending on the situation.
	 */
	PhysicsObject legsFrictionAdjuster;

	/** This object's physical body. 
	 */
	PhysicsObject torsoPhysicsObject;
	
	float friction;

private:
	/** Class initialization.
	 *
	 * @param widthScale the physics shape's width is this character's bounding box width times this value.
	 * This 
	 */
	void init(float widthScale);
};