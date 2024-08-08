/** PhysicsContactHandler.h
 *
 * Responsible for handling physics contacts (e.g.,collisions) between objects.
 *
 * @author Hans de Ruiter
 *
 * @copyright (c) 2022 by Kea Sigma Delta Limited, all rights reserved
 *
 * See License.txt for license.
 */

#pragma once

#include "box2d/box2d.h"

/** Handles physics collisions.
 * This may include calling methods on the objects that have collided, if they have custom
 * behaviour.
 */
class PhysicsContactHandler : public b2ContactListener {
public:
	PhysicsContactHandler();
	
	virtual ~PhysicsContactHandler();
	
	// WARNING: Do NOT create or destroy physics entities within this method! 
	void BeginContact(b2Contact *contact) override;
	
	// WARNING: Do NOT create or destroy physics entities within this method!
	void EndContact(b2Contact *contact) override;
	
	// WARNING: Do NOT create or destroy physics entities within this method!
	void PreSolve(b2Contact *contact, const b2Manifold *oldManifold) override;
	
	// WARNING: Do NOT create or destroy physics entities within this method!
	void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) override;
};