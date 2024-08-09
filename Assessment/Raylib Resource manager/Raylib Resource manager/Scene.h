/** Scene.h
 *
 * @author Hans de Ruiter
 *
 * @copyright (c) 2022-2024 by Kea Sigma Delta Limited, all rights reserved
 *
 * See License.txt for license.
 */

#pragma once

#include "CommandListener.h"

#include "DisplayOverlay.h"
#include "PhysicsContactHandler.h"

#include "raylib-cpp.hpp"
#include "box2d/b2_world.h"

#include <memory>
#include <list>

class Actor;
class DrawPhysics;
class TrackingCamera2D;

// Show the player avatar's bounding box
#define DEBUG_PLAYER_BBOX (1 << 0)

// Show the other actor's bouunding boxes
#define DEBUG_ACTOR_BBOXES (1 << 1)

// Show the scene's grid (e.g., tile-map grid)
#define DEBUG_SCENE_GRID (1 << 2)

// Show the scene's static collision shapes
#define DEBUG_SCENE_COLLISION_SHAPES (1 << 3)

// Show the physics debug info
#define DEBUG_SCENE_PHYSICS (1 << 4)

/** Base class for "scenes."
 * A scene is a "place where action occurs." It can be a title screen, a loading screen, 
 * a cut-scene, or an entire world in which players, actors, and other objects interact. 
 *
 * At a technical level, a scene loads and manages all objects that exist in a scene. It
 * processes or passes on user inputs to the relevant objects, and performs any simulation,
 * Artificial Intelligence (AI) calculations, etc., that are needed to display the scene.
 * Scenes can also have audio.
 *
 * Child classes can set the playerAvatar, which is the object that user input will be 
 * forwarded, unless the command methods (e.g., goRight()) are overridden.
 */
class Scene : public CommandListener {
public:
	Scene();
	
	virtual ~Scene();
	
	/** Loads the resources needed to display this scene (e.g., the entire game level).
	 *
	 * @throws std::runtime_error if something went wrong
	 */
	virtual void loadResources();
	
	/** Frees up the resources used in this scene.
	 * The default behaviour is to empty this object of all items, e.g., the actors, playerAvatar, etc.
	 */
	virtual void freeResources();
	
	/** Sets the scene's camera
	 */
	void setCamera(std::shared_ptr<TrackingCamera2D> &camera);
	
	/** Gets the scene's camera.
	 */
	std::shared_ptr<TrackingCamera2D>& getCamera();
	
	/** Get's the scene's 2D size.
	 * Defaults to the screen size if child-classes don't override it
	 */
	virtual raylib::Vector2 get2DSize() const;
	
	/** Called when the scene starts. 
	 * This should start any background music or sound, and perform any other initializaiton
	 * that isn't already handled by loadResources()
	 */
	virtual void start();
	
	/** Updates the scene.
	 * This should perform tasks such as:
	 * - Perform any Artificial Intelligence (AI) calculations
	 * - Update animation
	 * - Play music streams
	 * - Simulate the world, including triggering sound effects & maybe changing the music
	 *
	 * @param elapsedTime the time since the last frame in seconds
	 *
	 * @return std::shared_ptr<Scene> the next scene, or nullptr if this scene 
	 */
	virtual std::shared_ptr<Scene> update(float elapsedTime);
	
	/** Returns true if the program should quit.
	 */
	virtual bool shouldQuit();
	
	/** Draws the scene to the display.
	 */
	virtual void draw();
	
	/** Draws the debug overlay.
	 */
	virtual void drawDebug();

	/** Gets the player avatar.
	 */
	std::shared_ptr<const CommandListener> getPlayerAvatar() const;
	
	void goUp() override;
	
	void goLeft() override;
	
	void goRight() override;
	
	void goDown() override;
	
	void goNowhere() override;
	
	void doInteractWith();
	
	void doExit() override;
	
	void handleKeyPress(int key) override;
	
protected:
	/** Adds an actor to the list, and also adds its physics object.
	 *
	 * @param actor the actor to add
	 * @param position the actor's initial position in the scene
	 * @param velocity the actor's initial velocity
	 * @param addOnTop set to true to add to the end of the list (so it's drawn on top), and
	 * false to add it to the start of the list (so it's drawn behind everything else)
	 */
	void addActor(std::shared_ptr<Actor> actor, 
		const b2Vec2 &position, const b2Vec2 &velocity = b2Vec2(0.0f, 0.0f), bool addOnTop = true);
	
	/** Clears the list of actors.
	 */
	void clearActors();
	
	/** Set's the player avatar.
	 * Pass nullptr to remove a player without replacing.
	 */
	void setPlayerAvatar(std::shared_ptr<CommandListener> playerAvatar);

	/** Draws the scene's actors.
	 * The actors are drawn in order (i.e., from first to last, with the last appearing on top).
	 */
	void drawActors();

	/** Sets the display overlay.
	 */
	void setOverlay(std::shared_ptr<DisplayOverlay> overlay);

	/** Draws the overlay (if it exists).
	 */
	void drawOverlay() const;
	
	/** Get the physics world.
	 * This allows scenes to add their own bodies to the physics world.
	 */
	b2World& getPhysicsWorld();
	
	/** The scale factor between the physics world, and display coordinates. 
	 * The physics world is measured in metres, whereas everything is displayed in pixels.
	 *
	 * IMPORTANT: Set this *before* adding any objects to the physics world, or use the default.
	 * This will *NOT* rescale objects already in the world.
	 */
	float worldScale;
	
	/** Flags for debugging (can draw debug info to screen or print to the log).
	 */
	unsigned debugFlags;
	
	/** Controls the number of physics velocity iterations.
	 */
	int velocityIterations;
	
	/** Controls the number of physics position iterations.
	 */
	int positionIterations;
	
	/** The background music stream.
	 */
	std::shared_ptr<raylib::Music> musicStream;

private:
	
	/** The camera.
	 */
	std::shared_ptr<TrackingCamera2D> camera;
	
	
	/** The player's avatar. 
	 * By default, all user input will be forwarded to the object that the player (user) is
	 * controlling.
	 */
	std::shared_ptr<CommandListener> playerAvatar;
	
	/** List of actors in the scene, ordered from top to bottom.
	 * NOTE: The player should also be in this list.
	 */
	std::list< std::shared_ptr<Actor> > actors;
	
	/** The display overlay (if any)
	 */
	std::shared_ptr<DisplayOverlay> displayOverlay;

	/** Our physics simulation.
	 */
	b2World physicsWorld;
	
	/** Handles object contacts/collisions (tells objects when they touch something).
	 */
	PhysicsContactHandler physicsContactHandler;
	
	/** Object to draw the physics world (for debugging). 
	 */
	std::unique_ptr<DrawPhysics> drawPhysics; 
	
	/** A flag indicating it's time to exit back to the menu.
	 */
	bool exit;
};