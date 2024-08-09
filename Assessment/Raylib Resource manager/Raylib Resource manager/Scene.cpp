/** Scene.cpp
 *
 * See header file for details.
 */

#include "Scene.h"

#include "Actor.h"
#include "DrawPhysics.h"
#include "MainMenuScene.h"
#include "TrackingCamera2D.h"

#define DEFAULT_WORLDSCALE 50.0f
#define DEFAULT_GRAVITY DEFAULT_WORLDSCALE

static const b2Vec2 defaultGravity(0, DEFAULT_WORLDSCALE);

Scene::Scene() : physicsWorld(defaultGravity) {
	worldScale = DEFAULT_WORLDSCALE;
	
	physicsWorld.SetContactListener(&physicsContactHandler);
	
	// Suggested values from Box2D website
	// (https://box2d.org/documentation/md__d_1__git_hub_box2d_docs_hello.html).
	velocityIterations = 8;
	positionIterations = 3;
	
	
	debugFlags = 0;
	
	exit = false;
}
	
Scene::~Scene() {
	freeResources();
}
	
void Scene::loadResources() {
}
	
void Scene::freeResources() {
	playerAvatar.reset();
	clearActors();
}

void Scene::setCamera(std::shared_ptr<TrackingCamera2D> &camera) {
	this->camera = camera;
	camera->setSceneSize(get2DSize());
}

std::shared_ptr<TrackingCamera2D>& Scene::getCamera() {
	return camera;
}

raylib::Vector2 Scene::get2DSize() const {
	return raylib::Vector2((float)GetScreenWidth() / worldScale, (float)GetScreenHeight() / worldScale);
}
	
void Scene::start() {
}
	
std::shared_ptr<Scene> Scene::update(float elapsedTime) {
	if(musicStream && musicStream->IsPlaying()) {
		musicStream->Update();
	}
	
	physicsWorld.Step(elapsedTime, velocityIterations, positionIterations);
	
	for(auto actor = actors.begin(); actor != actors.end();) {
		bool isAlive = (*actor)->update(elapsedTime);
		if(isAlive) {
			++actor;
		} else {
			if(*actor == playerAvatar) {
				// The player is dead. Game over.
				exit = true;
			}
			actor = actors.erase(actor);
		}
	}
	
	if(camera) {
		camera->update(elapsedTime);
	}
	
	if(exit) {
		exit = false;
		return std::make_shared<MainMenuScene>();
	} else {
		return nullptr;
	}
}

bool Scene::shouldQuit() {
	return false;
}

void Scene::addActor(std::shared_ptr<Actor> actor, 
		const b2Vec2 &position, const b2Vec2 &velocity, bool addOnTop) {
	if(!actor) {
		return;
	}
	
	actor->createPhysicsBody(physicsWorld, worldScale, position, velocity);
	
	if(addOnTop) {
		actors.emplace_back(actor);
	} else {
		actors.emplace_front(actor);
	}
}

void Scene::setPlayerAvatar(std::shared_ptr<CommandListener> playerAvatar) {
	this->playerAvatar = playerAvatar;
}

std::shared_ptr<const CommandListener> Scene::getPlayerAvatar() const {
	return playerAvatar;
}

void Scene::clearActors() {
	for(auto &actor : actors) {
		actor->deletePhysicsBody();
	}
	actors.clear();
}
	
void Scene::draw() {
	if(camera) {
		camera->BeginMode();
	}
	
	ClearBackground(RAYWHITE);
		
	drawActors();
	
	if(debugFlags) {
		drawDebug();
	}
	
	if(camera) {
		camera->EndMode();
	}

	drawOverlay();
}


void Scene::drawDebug() {
	if((debugFlags & (DEBUG_PLAYER_BBOX | DEBUG_ACTOR_BBOXES)) != 0) {
		for(auto &actor : actors) {
			if(actor == playerAvatar &&(debugFlags & DEBUG_PLAYER_BBOX) != 0) {
				actor->drawBoundingBox(RED, worldScale);
			} else if((debugFlags & DEBUG_ACTOR_BBOXES) != 0) {
				actor->drawBoundingBox(ORANGE, worldScale);
			}
		}
	}
	if((debugFlags & DEBUG_SCENE_PHYSICS) != 0) {
		if(!drawPhysics) {
			drawPhysics.reset(new DrawPhysics(worldScale));
			drawPhysics->SetFlags(b2Draw::e_shapeBit);
			physicsWorld.SetDebugDraw(drawPhysics.get());
		}
		physicsWorld.DebugDraw();
	}
}
	
void Scene::goUp() {
	if(playerAvatar) {
		playerAvatar->goUp();
	}
}
	
void Scene::goLeft() {
	if(playerAvatar) {
		playerAvatar->goLeft();
	}
}
	
void Scene::goRight() {
	if(playerAvatar) {
		playerAvatar->goRight();
	}
}
	
void Scene::goDown() {
	if(playerAvatar) {
		playerAvatar->goDown();
	}
}

void Scene::goNowhere() {
	if(playerAvatar) {
		playerAvatar->goNowhere();
	}
}
	
void Scene::doInteractWith() {
	if(playerAvatar) {
		playerAvatar->doInteractWith();
	}
}

void Scene::doExit() {
	exit = true;
}

void Scene::handleKeyPress(int key) {
	// Toggle the debug flags on and off
	switch(key) {
	case '1':
		debugFlags = debugFlags ^ DEBUG_PLAYER_BBOX;
		break;
	case '2':
		debugFlags = debugFlags ^ DEBUG_ACTOR_BBOXES;
		break;
	case '3':
		debugFlags = debugFlags ^ DEBUG_SCENE_GRID;
		break;
	case '4':
		debugFlags = debugFlags ^ DEBUG_SCENE_COLLISION_SHAPES;
		break;
	case '5':
		debugFlags = debugFlags ^ DEBUG_SCENE_PHYSICS;
		break;
	default:
		;
	}
}

void Scene::drawActors() {
	if(actors.size()) {
		for(auto &actor: actors) {
			actor->draw(worldScale);
		}
	}
}

void Scene::setOverlay(std::shared_ptr<DisplayOverlay> overlay) {
	displayOverlay = overlay;
}

void Scene::drawOverlay() const {
	if(displayOverlay) {
		displayOverlay->draw(*this);
	}
}

b2World& Scene::getPhysicsWorld() {
	return physicsWorld;
}