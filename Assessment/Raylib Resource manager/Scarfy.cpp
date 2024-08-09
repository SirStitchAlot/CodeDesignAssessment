/** Scarfy.cpp
 *
 * See header file for details.
 */

#include "Scarfy.h"

#include "Common.h"
#include "ResourceManager.h"

#include <cfloat>

static const int standingFrame = 2;
static const int jumpUpFrame = 3;
static const int jumpDownFrame = 4;
static const int footstepFrame1 = 1;
static const int footstepFrame2 = 4;

#define PHYSICS_WIDTH 0.7f // Width of physics object vs Scarfy's bounding box

static bool isFootstepFrame(int frameIndex) {
	
	if(frameIndex == footstepFrame1 || frameIndex == footstepFrame2) {
		return true;
	} else {
		return false;
	}
}

Scarfy::Scarfy() : 
	CharacterActor(PHYSICS_WIDTH),
	texture(DATADIR "scarfy.png"),
	footstepSound(SoundManager::load(DATADIR "Single-footstep-in-grass-A-www.fesliyanstudios.com.mp3")),
	landingSound(SoundManager::load(DATADIR "Single-footstep-in-grass-B-www.fesliyanstudios.com.mp3")) {
	
	numFrames = 6;
	frameWidth = texture.width / numFrames;
	frameRect = raylib::Rectangle(0.0f, 0.0f, (float)frameWidth, (float)texture.height);
	
	frameDelay = 5.0f / 60.0f;
	frameDelayCounter = 0;
	frameIndex = 0;

	// Scafy has a wallet, initially containing this much...
	walletValue = 0;
}
	
Scarfy::~Scarfy() {
}
	
bool Scarfy::update(float elapsedTime) {
	const auto &velocity = getVelocity();
	bool scarfyMoving = isMoving();
	
	if(isOnGround && !wasOnGround) {
		landingSound->Play();
	}
	
	frameDelayCounter += elapsedTime;
	if(frameDelayCounter > frameDelay) {
		frameDelayCounter = 0;
		
		if(scarfyMoving) {
			if(isOnGround) {
				++frameIndex;
				frameIndex %= numFrames;
				
				if(isFootstepFrame(frameIndex)) {
					footstepSound->Play();
				}
			} else {
				if(velocity.y < 0) {
					frameIndex = jumpUpFrame;
				} else {
					frameIndex = jumpDownFrame;
				}
			}
			frameRect.x = (float) frameWidth * frameIndex;
		} else if (isOnGround) {
			frameIndex = standingFrame;
			frameRect.x = (float) frameWidth * frameIndex;
		}
	}
	
	wasOnGround = isOnGround;
	
	return true;
}
	
void Scarfy::draw(float worldScale) {
	raylib::Vector2 ulPosition = getUpperLeftPosition(worldScale);
	if(facingRight) {
		frameRect.width = (float)frameWidth;
	} else {
		frameRect.width = (float)-frameWidth;
	}
	raylib::Rectangle destRect(ulPosition.x, ulPosition.y, 
		frameWidth / worldScale, texture.height / worldScale); 
	DrawTexturePro(texture, frameRect, destRect, raylib::Vector2(0.0f,0.0f), 0.0f, WHITE);
}
	
raylib::Rectangle Scarfy::getBoundingBox(float worldScale) {
	raylib::Vector2 ulPosition = getUpperLeftPosition(worldScale);
	return raylib::Rectangle(ulPosition.x, ulPosition.y, 
		(float)frameWidth / worldScale, (float)texture.height / worldScale);
}

raylib::Vector2 Scarfy::getUpperLeftPosition(float worldScale) {
	const b2Vec2 &position = getPosition();
	return raylib::Vector2(
		position.x - frameWidth / (2.0f * worldScale),
		position.y - texture.height / worldScale);
}
