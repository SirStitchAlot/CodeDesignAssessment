/** TileMap2D.h
 *
 * Handles 2D tile maps in Tiled JSON format (*.tmj).
 *
 * @author Hans de Ruiter
 *
 * @copyright (c) 2022 by Kea Sigma Delta Limited, all rights reserved
 *
 * See License.txt for license.
 *
 * Credit:
 * Rob Loach's raylib-tileson code was helpful as a reference to get this working.
 * https://github.com/RobLoach/raylib-tileson
 */

#pragma once

#include "raylib-cpp.hpp"

#include <string>
#include <memory>
#include <unordered_map>

namespace tson {
	class Layer;
	class Map;
};

class b2World;

class TileMap2D
{
public:
	/** Construct a new tile map from a Tiled JSON file (*.tmj).
	 * It can also load LZMA compressed files. Simply add .lzma to the filename.
	 *
	 * @param fileName the filename of the tile-map to load
	 * @param groundLayerName the name of the layer containing the surface that the 
	 * player and actors walk on (or collide with)
	 */
	TileMap2D(const std::string &fileName, const std::string &groundLayerName = "Ground");

	virtual ~TileMap2D();
	
	/** Draw this map to screen.
	 *
	 * IMPORTANT: You *MUST* have called BeginMode2D() with the chosen camera before
	 * calling this. The camera is used to calculate the parallax effect.
	 *
	 * @param camera the 2D camera looking at the scene
	 * @param worldScale the scale factor that converts from world coordinates to display coordinates
	 * @param tint the colour to tint the entire map (use WHITE to draw in original
	 * colours)
	 */
	void draw(const raylib::Camera2D &camera, 
		float worldScale, const raylib::Color &tint = WHITE);
	
	/** Draws the ground layer's grid.
	 */
	void drawGrid(const raylib::Camera2D &camera, 
		float worldScale, const raylib::Color &color = DARKGRAY);
	
	/** Draws the ground layer's collision shapes.
	 */
	void drawCollisionShapes(const raylib::Camera2D &camera, 
		float worldScale, const raylib::Color &color = BLUE);
	
	/** Update animations.
	 *
	 * @param elapsedTime in seconds
	 */
	void updateAnimations(float elapsedTime);
	
	/** Generate the physics objects for this tile-map.
	 *
	 * @param physicsWorld the physics world to add this tile map to
	 * @param worldScale the scale factor from world to display coordinates
	 */
	void generatePhysicsObjects(b2World &physicsWorld, float worldScale);
	
	/** The tile map's size in pixels.
	 */
	raylib::Vector2 getMapSize() const;

private:	
	/** Draws a single layer.
	 */
	void drawLayer(tson::Layer &layer, const raylib::Camera2D &camera, 
		float worldScale, const raylib::Vector2 &offset, const raylib::Color &tint);
	
	void drawTileLayer(tson::Layer &layer, const raylib::Camera2D &camera, 
		float worldScale, const raylib::Vector2 &offset, const raylib::Color &tint);
	
	void drawImageLayer(tson::Layer &layer, const raylib::Camera2D &camera, 
		float worldScale, const raylib::Vector2 &offset, const raylib::Color &tint);
	
	/** Calculates a layer's offset.
	 *
	 * @param layer the layer
	 * @param camera the camera viewing the layer
	 * @param worldScale the scale factor that converts from world coordinates to display coordinates
	 * @param offset a base offset to add to the layer's own offsets
	 */
	const raylib::Vector2 calcLayerOffset(
		tson::Layer &layer, const raylib::Camera2D &camera,  
		float worldScale, const raylib::Vector2 &offset = raylib::Vector2::Zero());
	
	/** Loads the given image as a texture.
	 * NOTE: This can be safely called on the same image multiple times. Image
	 * files will only be loaded once.
	 *
	 * @param baseDir the base directory that all image files are relative to
	 * @param fileName the image's file name
	 */
	void loadImage(const std::string &baseDir, const std::string &fileName);
	
	std::unique_ptr<tson::Map> tileMap;
	
	std::unordered_map<std::string, std::shared_ptr<raylib::Texture>> textures;
	
	/** Which layer is the ground that actors walk on and/or collide with?
	 */
	unsigned groundLayerIdx;
};