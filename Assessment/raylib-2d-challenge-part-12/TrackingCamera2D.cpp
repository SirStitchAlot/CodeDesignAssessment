/** TrackingCamera2D.cpp
 *
 * See header file for details.
 */

#include "TrackingCamera2D.h"

#define DEFAULT_P_FACTOR 0.3f
#define DEFAULT_I_FACTOR 0.15f
#define DEFAULT_TARGET_RADIUS 100.0f

static inline raylib::Vector2 calcAbsTargetOffset(int screenWidth, float zoom) {
	return raylib::Vector2(screenWidth / (6.0f * zoom), 0.0f);
}

TrackingCamera2D::TrackingCamera2D() {
	init();
}

TrackingCamera2D::TrackingCamera2D(::Vector2 offset, ::Vector2 target,
            float rotation, float zoom) : 
			raylib::Camera2D(offset, target, rotation, zoom) {
	position = target;
	init();
}

void TrackingCamera2D::setTarget(std::shared_ptr<Actor> target) {
	this->target = target;
}
	
std::shared_ptr<Actor> TrackingCamera2D::getTarget() {
	return target;
}

void TrackingCamera2D::setSceneSize(const raylib::Vector2 &sceneSize) {
	this->sceneSize = sceneSize;
}

bool TrackingCamera2D::update(float elapsedTime) {
	if(!target) {
		return true;
	}
	
	// Using the photography "rule of thirds" horizontally,
	// so calculate the offset based on the direction that the
	// target is moving
	float zoom = GetZoom();
	const b2Vec2 &targetVelocityB2D = target->getVelocity();
	raylib::Vector2 targetVelocity(targetVelocityB2D.x, targetVelocityB2D.y);
	int screenWidth = GetScreenWidth();
	if(targetVelocity.x > 0) {
		targetOffset = -calcAbsTargetOffset(screenWidth, zoom); 
	} else if(targetVelocity.x < 0) {
		targetOffset = calcAbsTargetOffset(screenWidth, zoom);
	}

	// This is a basic PI controller. See:
	// https://en.wikipedia.org/wiki/PID_controller
	const b2Vec2 &targetPositionB2 = target->getPosition();
	raylib::Vector2 targetPosition(targetPositionB2.x, targetPositionB2.y);
	auto error = targetPosition - (position + targetOffset);
	
	if(sceneSize.Length() > 0) {
		// Clamp the camera to the scene size
		auto cameraOffset = GetOffset();
		cameraOffset.x /= zoom;
		cameraOffset.y /= zoom;
		if(position.x - cameraOffset.x <= 0) { 
			position.x = cameraOffset.x; 
			if(error.x < 0) { 
				error.x = 0.0f; 
				integral.x = 0.0f;
				targetVelocity.x = 0.0f;
			}
		}
		if(position.y - cameraOffset.y <= 0) { 
			position.y = cameraOffset.y; 
			if(error.y < 0) { 
				error.y = 0.0f; 
				integral.y = 0.0f;
				targetVelocity.y = 0.0f;
			}
		}
		if(position.x + cameraOffset.x >= sceneSize.x) { 
			position.x = sceneSize.x - cameraOffset.x; 
			if(error.x > 0) { 
				error.x = 0.0f; 
				integral.x = 0.0f;
				targetVelocity.x = 0.0f;
			}
		}
		if(position.y + cameraOffset.y >= sceneSize.y) { 
			position.y = sceneSize.y - cameraOffset.y;
			if(error.y > 0) { 
				error.y = 0.0f; 
				integral.y = 0.0f;
				targetVelocity.y = 0.0f;
			}
		}
	}
	
	if(error.Length() < targetRadius) {
		integral = raylib::Vector2::Zero();
	}
	integral += error;
	velocity = (error * propFactor + integral * integralFactor
		+ targetVelocity) * elapsedTime;
	
	this->position += this->velocity;
	
	SetTarget(this->position);
	
	return true;
}

void TrackingCamera2D::init() {
	sceneSize = raylib::Vector2(0.0f, 0.0f);
	propFactor = DEFAULT_P_FACTOR;
	integralFactor = DEFAULT_I_FACTOR;
	targetRadius = DEFAULT_TARGET_RADIUS;
	integral = raylib::Vector2(0.0f, 0.0f);
	
	int screenWidth = GetScreenWidth();
	targetOffset = -calcAbsTargetOffset(screenWidth, zoom);
}

