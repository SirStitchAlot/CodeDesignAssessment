/** Scarfy.h
 *
 * @author Hans de Ruiter
 *
 * @copyright (c) 2022 by Kea Sigma Delta Limited, all rights reserved
 *
 * See License.txt for license.
 */

#pragma once

#include "CharacterActor.h"

#include "raylib-cpp.hpp"

/** Our scarfy character.
 */
class Scarfy : public CharacterActor {
public:
	/** Create Scarfy.
	 */
	Scarfy();
	
	virtual ~Scarfy();
	
	virtual bool update(float elapsedTime);
	
	virtual void draw(float worldScale);
	
	virtual raylib::Rectangle getBoundingBox(float worldScale);
	
private:
	/** Calculates the position of this object's upper-left corner in world coordinates.
	 */
	raylib::Vector2 getUpperLeftPosition(float worldScale);

	raylib::Texture2D texture;
	
	std::shared_ptr<raylib::Sound> footstepSound;
	std::shared_ptr<raylib::Sound> landingSound;
	
	// Animation state
	unsigned numFrames;
	int frameWidth;
	float frameDelay;
	float frameDelayCounter;
	unsigned frameIndex;
	raylib::Rectangle frameRect;
};