/** TileMap2DScene.h
 *
 * Base class for 2D scenes with tilemaps.
 *
 * @author Hans de Ruiter
 *
 * @copyright (c) 2022 by Kea Sigma Delta Limited, all rights reserved
 *
 * See License.txt for license.
 */

#pragma once

#include "Scene.h"
#include "TileMap2D.h"

class TileMap2D;

class TileMap2DScene : public Scene {
public:
	TileMap2DScene();
	
	/** Loads the tile-map from file.
	 *
	 * @param fileName the filename of the tile-map to load
	 * @param groundLayerName the name of the layer containing the surface that the 
	 * player and actors walk on (or collide with)
	 */
	TileMap2DScene(const std::string &fileName, const std::string &groundLayerName = "Ground");
	
	virtual ~TileMap2DScene();
	
	virtual raylib::Vector2 get2DSize() const;
	
	virtual std::shared_ptr<Scene> update(float elapsedTime);
	
	virtual void draw();
	
protected:
	/** The tile-map.
	 * Child-classes should set this
	 */
	std::shared_ptr<TileMap2D> tileMap;
};
