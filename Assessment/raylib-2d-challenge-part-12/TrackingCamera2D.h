/** TrackingCamera2D.h
 *
 * @author Hans de Ruiter
 *
 * @copyright (c) 2022 by Kea Sigma Delta Limited, all rights reserved
 *
 * See License.txt for license.
 */

#pragma once

#include <Actor.h>

#include "raylib-cpp.hpp"

#include <memory>

class Actor;

/** A camera for 2D scenes that will follow an actor.
 */
class TrackingCamera2D : public raylib::Camera2D {
public:
    TrackingCamera2D();
	
	/** Create a tracking 2D camera with the given parameters.
	 *
	 * @param offset offset to the camera's origin in pixels (this is usually the screen centre)
	 * @param target the initial camera target (i.e., where the camera is looking) in world coordinates
	 * @param rotation the camera's rotation (anti-clockwise) in radians
	 * @param zoom a zoom factor from world coordinates to screen coordinates (should normally be
	 * the scene's worldScale, unless you want to add additional zoom)
	 */
    TrackingCamera2D(::Vector2 offset, ::Vector2 target,
            float rotation = 0.0f, float zoom = 1.0f);
	
	TrackingCamera2D(const TrackingCamera2D &camera) = delete;
	
	/** Sets the actor that the camera should focus on.
	 */
	void setTarget(std::shared_ptr<Actor> target);
	
	/** Gets the actor that the camera currently focuses on.
	 */
	std::shared_ptr<Actor> getTarget();
	
	/** Sets the scene size in world coordinates.
	 * NOTE: This is called when the camera is added to a scene.
	 */
	void setSceneSize(const raylib::Vector2 &sceneSize);
	
	virtual bool update(float elapsedTime);

private:
	/** Performs basic camera initialization.
	 */
	void init();
	
	std::shared_ptr<Actor> target;
	
	/** The scene's size.
	 */
	raylib::Vector2 sceneSize;
	
	/** The offset from the screen centre to the preferred target 
	 * location. 
	 */
	raylib::Vector2 targetOffset;
	
	/** The camera's position in the scene.
	 */
	raylib::Vector2 position;
	
	/** The camera's velocity.
	 */
	raylib::Vector2 velocity;
	
	/** The PI controller's Proportional factor.
	 */
	float propFactor;
	
	/** The PI controller's Integral factor.
	 */
	float integralFactor;
	
	/** The radius around the target that's considered "close enough."
	 * This is used to prevent overshoot & oscillations from the 
	 * controller.
	 */
	float targetRadius;
	
	/** The integral of the "control error" (differenc between where
	 * the camera should be pointed and where it is).
	 */
	raylib::Vector2 integral;
};