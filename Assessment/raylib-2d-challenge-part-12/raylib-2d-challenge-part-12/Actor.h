/** Actor.h
 *
 * @author Hans de Ruiter
 *
 * @copyright (c) 2022 by Kea Sigma Delta Limited, all rights reserved
 *
 * See License.txt for license.
 */

#pragma once

#include "CommandListener.h"

#include "box2d/box2d.h"
#include "Rectangle.hpp"

#include <climits>

class Collectable;

// Used to indicate that an actor has no wallet (and therefore cannot collect/earn anything)
#define ACTOR_NO_WALLET INT_MIN

/** Base class for actors in scenes. 
 * An actor is anything in the scene that moves, or is not part of the background.
 */
class Actor : public CommandListener {
public:
	Actor();
	
	virtual ~Actor();
	
	/** Updates this actor's state.
	 * This could include the animation state, triggering of sounde effects, etc.
	 *
	 * IMPORTANT: Physics simulation is handled at the scene level.
	 *
	 * @return bool true if this actor still exists, and false if it has "exited
	 * the scene, never to return" (i.e., dead)
	 */
	virtual bool update(float elapsedTime);
	
	/** Draws this actor to the display.
	 *
	 * @param worldScale the scale factor from world coordinates to display coordinates 
	 * NOTE: Exclude additional camera zoom, or your camera zoom effect may not work
	 */
	virtual void draw(float worldScale);

	/** Collects a collectable item, if this actor is capable of collecting them.
	 * 
	 * NOTE: The default implementation is to not collect items.
	 * 
	 * @param collectable the collectable item to collect
	 * 
	 * @return bool true if this actor collected the item, false otherwise
	 */
	virtual bool collect(Collectable *collectable);

	/** Gets the value of the actor's wallet (i.e., the value of all collected items). 
	 */
	int getWalletValue() const;

	
	/** Draw's this actor's bounding box.
	 *
	 * @param color the colour to draw
	 *
	 * @param worldScale the scale factor from world coordinates to display coordinates 
	 * NOTE: Exclude additional camera zoom, or your camera zoom effect may not work
	 */
	virtual void drawBoundingBox(const Color &color, float worldScale);
	
	/** Returns this actor's bounding box in world coordinages (i.e., the smallest 
	 * rectangle that contains this actor, including all its sub-components).
	 *
	 * @param worldScale the scale factor from world coordinates to display coordinates 
	 * NOTE: Exclude additional camera zoom, or your camera zoom effect may not work
	 */
	virtual raylib::Rectangle getBoundingBox(float worldScale);
	
	/** Gets the actor's position in the world.
	 */
	const b2Vec2& getPosition() const;
	
	/** Sets the actor's position.
	 */ 
	void setPosition(const b2Vec2 &position);
	
	/** Gets the actor's velocity.
	 */
	const b2Vec2& getVelocity() const; 
	
	/** Sets the actor's velocity.
	 */
	void setVelocity(const b2Vec2 &velocity);
	
	/** Returns true if the actor is moving.
	 */
	bool isMoving() const;

	/** This actor has just died.
	 * 
	 * NOTE: Child-classes can override this method if they want to show a dying animation 
	 * before completely disappearing. Otherwise, it'll mark this actor as dead and remove it
	 * from the scene.
	 */
	virtual void die();
	
	/** Create this actor's physics body.
	 *
	 * NOTES: 
	 * - The base class silently creates nothing. Child-classes should implement this method.
	 * - The physics body pointer should be written to the physicsBody field (e.g., 
	 *   physicsBody = world.CreateBody(&bodyDef);
	 *
	 * @param world the physics world to add the actor to
	 * @param worldScale the scale factor that converts from world coordinates to display coordinates
	 * @param position the actor's initial position in the scene
	 * @param velocity the actor's initial velocity
	 */
	virtual void createPhysicsBody(b2World &world, float worldScale, 
		const b2Vec2 &position, const b2Vec2 &velocity = b2Vec2(0.0f, 0.0f));
	
	/** Deletes the physics body, thereby removing this actor from a physics world.
	 */
	void deletePhysicsBody();
	
	
public:
	/** Is this actor currently on the ground?.
	 * This is set by the physics simluation.
	 */
	bool isOnGround;
	
	/** The previous state of isOnGround.
	 * This should be updated at the end of the update() method.
	 */
	bool wasOnGround;

protected:
	bool isAlive;

	/** The actor's physics body.
	 */
	b2Body *physicsBody;

	/** How much is in this actor's wallet?
	 * 
	 * Set to ACTOR_NO_WALLET if the actor has no wallet (and therefore can't collect/earn). 
	 * This is the default value
	 */
	int walletValue;
};
