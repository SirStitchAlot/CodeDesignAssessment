/** TileMap2DScene.cpp
 *
 * See header file for details.
 */

#include "TileMap2DScene.h"

#include "TileMap2D.h"
#include "TrackingCamera2D.h"

const static TrackingCamera2D defaultCamera;

TileMap2DScene::TileMap2DScene() {
}

TileMap2DScene::TileMap2DScene(const std::string &fileName, const std::string &groundLayerName) :
		tileMap(std::make_shared<TileMap2D>(fileName, groundLayerName)) {
}

TileMap2DScene::~TileMap2DScene() {
}

raylib::Vector2 TileMap2DScene::get2DSize() const {
	if(tileMap) {
		return tileMap->getMapSize() / worldScale;
	} else {
		return Scene::get2DSize();
	}
}

std::shared_ptr<Scene> TileMap2DScene::update(float elapsedTime) {
	if(tileMap) {
		tileMap->updateAnimations(elapsedTime);
	}
	
	return Scene::update(elapsedTime);
}

void TileMap2DScene::draw() {
	ClearBackground(RAYWHITE);
	
	auto &camera = getCamera();
	if(camera) {
		camera->BeginMode();
	}
	if(tileMap) {
		auto &currCamera = camera ? *camera : defaultCamera;
		tileMap->draw(currCamera, worldScale, WHITE);
	}
	
	drawActors();
	
	if(debugFlags) {
		if((debugFlags & DEBUG_SCENE_GRID) != 0) {
			if(tileMap) {
				auto &currCamera = camera ? *camera : defaultCamera;
				tileMap->drawGrid(currCamera, worldScale, DARKGRAY);
			}
		}
		if((debugFlags & DEBUG_SCENE_COLLISION_SHAPES) != 0) {
			if(tileMap) {
				auto &currCamera = camera ? *camera : defaultCamera;
				tileMap->drawCollisionShapes(currCamera, worldScale, BLUE);
			}
		}
		drawDebug();
	}
	
	if(camera) {
		camera->EndMode();
	}

	drawOverlay();
}
