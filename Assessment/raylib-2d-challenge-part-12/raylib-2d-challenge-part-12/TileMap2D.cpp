/** TileMap2D.cpp
 *
 * See the header file for details.
 */

#include "TileMap2D.h"

#include "ResourceManager.h"
#include "TileMap2DLoader.h"


#include "box2d/box2d.h"

using namespace raylib;

static raylib::Rectangle toRayLibRect(tson::Rect rect) {
	return raylib::Rectangle((float)rect.x, (float)rect.y, (float)rect.width, (float)rect.height);
}

TileMap2D::TileMap2D(const std::string &fileName, const std::string &groundLayerName) {
	tileMap = loadTileMap(fileName);
	
	if(tileMap->getStatus() != tson::ParseStatus::OK) {
		throw std::runtime_error(TextFormat("Failed to load tile-map %s. Error: %s", 
			fileName.c_str(), tileMap->getStatusMessage().c_str()));
	}
	
	// Load the images
	std::string baseDir = GetDirectoryPath(fileName);
	auto &layers = tileMap->getLayers();
	for (const auto& layer : layers) {
        if (layer.getType() == tson::LayerType::ImageLayer) {
            loadImage(baseDir, layer.getImage());
        }
    }
	auto tilesets = tileMap->getTilesets();
	for (auto& tileset : tilesets) {
		auto &image = tileset.getImage();
		if(!image.empty()) {
			loadImage(baseDir, image.string());
		} else {
			for(const auto& tile : tileset.getTiles()) {
				auto &image = tile.getImage();
				if(!image.empty()) {
					loadImage(baseDir, image.string());
				}
			}
		}
    }
	
	// Find the ground layer
	groundLayerIdx = 0;
	for(auto &layer : layers) {
		if(groundLayerName.compare(layer.getName()) == 0) {
			// Found
			break;
		}
		++groundLayerIdx;
	}
	
	bool foundGround = groundLayerIdx < layers.size();
	if(!foundGround) {
		throw std::runtime_error(TextFormat("Couldn't find ground layer %s in tile-map %s", 
			groundLayerName.c_str(), fileName.c_str()));
	}
}

TileMap2D::~TileMap2D() {
}

void TileMap2D::draw(const raylib::Camera2D &camera, 
		float worldScale, const raylib::Color &tint) {
	auto &layers = tileMap->getLayers();
	for(auto &layer : layers) {
		drawLayer(layer, camera, worldScale, raylib::Vector2(0,0), tint);
	}
}

void TileMap2D::drawGrid(const raylib::Camera2D &camera, 
		float worldScale, const raylib::Color &color) {
	auto &layers = tileMap->getLayers();
	if(groundLayerIdx < layers.size()) {
		auto &layer = layers[groundLayerIdx];
		raylib::Vector2 offset = calcLayerOffset(layer, camera, worldScale);
		auto mapSize = layer.getSize();
		auto tileSize = tileMap->getTileSize();
		
		const float lineThickness = 2 / worldScale;
		for(int y = 0; y < mapSize.y; ++y) {
			float yPos = offset.y + y * tileSize.y / worldScale;
			raylib::Vector2 startPos(offset.x, yPos);
			raylib::Vector2 endPos(offset.x + mapSize.x * tileSize.x, yPos);
			DrawLineEx(startPos, endPos, lineThickness, color); 
		}
		for(int x = 0; x < mapSize.x; ++x) {
			float xPos = offset.x + x * tileSize.x / worldScale;
			raylib::Vector2 startPos(xPos, offset.y);
			raylib::Vector2 endPos(xPos, offset.y + mapSize.y * tileSize.y);
			DrawLineEx(startPos, endPos, lineThickness, color); 
		}
	}
}

void TileMap2D::drawCollisionShapes(const raylib::Camera2D &camera, 
		float worldScale, const raylib::Color &color) {
	const float lineThickness = 2 / worldScale;
	auto &layers = tileMap->getLayers();
	if(groundLayerIdx < layers.size()) {
		auto &layer = layers[groundLayerIdx];
		raylib::Vector2 offset = calcLayerOffset(layer, camera, worldScale);
		for (auto& [pos, tileObject] : layer.getTileObjects()) {
			tson::Tile* tile = tileObject.getTile();
			auto tilePos = tileObject.getPosition();
			raylib::Vector2 tileOffset = raylib::Vector2(tilePos.x, tilePos.y) / worldScale + offset;
			auto &objectGroup = tile->getObjectgroup();
			auto collisionShapes = objectGroup.getObjects();
			for(auto &shape : collisionShapes) {
				auto &shapePos = shape.getPosition();
				raylib::Vector2 shapeOffset = raylib::Vector2((float)shapePos.x, (float)shapePos.y) / worldScale + tileOffset;
				
				switch(shape.getObjectType()) {
				case tson::ObjectType::Polygon:
				case tson::ObjectType::Polyline: // NOTE: Treating a polyline as a polygon for collision
				{
					auto &polygon = shape.getPolygons();
					if(polygon.size() > 0) {
						auto &lastPos = polygon[polygon.size() - 1];
						raylib::Vector2 prevPos = raylib::Vector2((float)lastPos.x, (float)lastPos.y) / worldScale + shapeOffset;
						for(auto &currPoint : polygon) {
							raylib::Vector2 currPos = raylib::Vector2((float)currPoint.x, (float)currPoint.y) / worldScale + shapeOffset;
							DrawLineEx(prevPos, currPos, lineThickness, color);
							prevPos = currPos;
						}
					}
					break;
				}
				case tson::ObjectType::Rectangle:
				{
					auto &rectSize = shape.getSize();
					raylib::Rectangle rect((float)shapeOffset.x, (float)shapeOffset.y, 
						(float)rectSize.x / worldScale, (float)rectSize.y / worldScale);
					DrawRectangleLinesEx(rect, lineThickness, color);
					break;
				}
				default:
					TraceLog(LOG_ERROR, "Error: Unrecognized collision shape object type: %u", shape.getObjectType());
				}
			}
		}
	}
}

void TileMap2D::updateAnimations(float elapsedTime) {
	// ##### FIXME! ###### To-do
}

void TileMap2D::generatePhysicsObjects(b2World &physicsWorld, float worldScale) {
	// Create vertices variable here to avoid repeated construction/destruction overhead (for every tile)
	std::vector<b2Vec2> vertices;
	const float defaultFriction = 1.0f;
	
	auto &layers = tileMap->getLayers();
	auto tileSizeTson = tileMap->getTileSize();
	b2Vec2 tileSize_2(tileSizeTson.x / (2.0f * worldScale), tileSizeTson.y / (2.0f * worldScale));
	if(groundLayerIdx < layers.size()) {
		auto &layer = layers[groundLayerIdx];
		
		auto layerOffsetTson = layer.getOffset();
		b2Vec2 layerOffset(layerOffsetTson.x / worldScale, layerOffsetTson.y / worldScale);
		
		
		for (auto& [pos, tileObject] : layer.getTileObjects()) {
			tson::Tile* tile = tileObject.getTile();
			auto tilePos = tileObject.getPosition();
			b2Vec2 tileOffset = b2Vec2(tilePos.x / worldScale, tilePos.y / worldScale) + layerOffset;
			
			auto &objectGroup = tile->getObjectgroup();
			auto collisionShapes = objectGroup.getObjects();
			
			for(auto &shape : collisionShapes) {
				auto &shapePos = shape.getPosition();
				b2Vec2 shapeOffset = b2Vec2((float)shapePos.x / worldScale, 
					(float)shapePos.y / worldScale) + tileOffset;
				
				b2BodyDef bodyDef;
				bodyDef.type = b2_staticBody;
				bodyDef.fixedRotation = true;
				bodyDef.position = shapeOffset + tileSize_2;
				b2Body *tileBody = physicsWorld.CreateBody(&bodyDef);
				b2Fixture *tileFixture = NULL;
				
				float tileFriction = defaultFriction;
				
				switch(shape.getObjectType()) {
				case tson::ObjectType::Polygon:
				case tson::ObjectType::Polyline: // NOTE: Treating a polyline as a polygon for collision
				{
					vertices.clear();
					auto &polygon = shape.getPolygons();
					if(polygon.size() >= 3) {
						for(auto &currPoint : polygon) {
							b2Vec2 currPos = b2Vec2((float)currPoint.x / worldScale, 
								(float)currPoint.y / worldScale) - tileSize_2;
							vertices.push_back(currPos);
						}
						b2ChainShape shape;
						shape.CreateLoop(vertices.data(), (int32)vertices.size());
						tileFixture = tileBody->CreateFixture(&shape, 0.0f);
					}
					break;
				}
				case tson::ObjectType::Rectangle:
				{
					auto &rectSize = shape.getSize();
					raylib::Rectangle rect((float)shapeOffset.x, (float)shapeOffset.y, 
						(float)rectSize.x / worldScale, (float)rectSize.y / worldScale);
					b2PolygonShape shape;
					shape.SetAsBox((float)rectSize.x / (2.0f * worldScale), 
						(float)rectSize.y / (2.0f * worldScale));
					tileFixture = tileBody->CreateFixture(&shape, 0.0f);
					break;
				}
				default:
					TraceLog(LOG_ERROR, "Error: Unrecognized collision shape object type: %u", shape.getObjectType());
				}
				
				if(tileFixture) {
					tileFixture->SetFriction(tileFriction);
				}
			}
		}
	}
}

raylib::Vector2 TileMap2D::getMapSize() const {
	auto size = tileMap->getSize();
	auto tileSize = tileMap->getTileSize();
	return raylib::Vector2((float)(size.x * tileSize.x), (float)(size.y * tileSize.y));
}

void TileMap2D::drawLayer(
		tson::Layer &layer, const raylib::Camera2D &camera,   
		float worldScale,const raylib::Vector2 &offset, const raylib::Color &tint) {
	raylib::Vector2 newOffset = calcLayerOffset(layer, camera, worldScale, offset);
	
	switch(layer.getType()) {
	case tson::LayerType::TileLayer:
		drawTileLayer(layer, camera, worldScale, newOffset, tint);
		break;
	case tson::LayerType::ImageLayer:
		drawImageLayer(layer, camera, worldScale, newOffset, tint);
		break;
	case tson::LayerType::Group:
	{
		for(auto &subLayer : layer.getLayers()) {
			drawLayer(subLayer, camera, worldScale, newOffset, tint);
		}
		break;
	}
	default:
		// Fail
		auto *errMsg = TextFormat("ERROR: Unsupported Tileson layer type %u", 
			layer.getType());
		throw std::runtime_error(errMsg);
	}
}
	
void TileMap2D::drawTileLayer(
		tson::Layer &layer, const raylib::Camera2D &camera,   
		float worldScale, const raylib::Vector2 &offset, const raylib::Color &tint) {
	for (auto& [pos, tileObject] : layer.getTileObjects()) {
		tson::Tile* tile = tileObject.getTile();
		tson::Tileset *tileset = tile->getTileset();
		auto tileImage = tileset->getImage();
		auto imagePath = tileImage.empty() ? tile->getImage() : tileImage;
		auto texIter = textures.find(imagePath.string());
		if(texIter == textures.end()) {
			auto *errMsg = TextFormat("ERROR: Tile-map is missing image: %s\n", 
				imagePath.string().c_str());
			throw std::runtime_error(errMsg);
		}
		auto &texture = texIter->second;
		
		raylib::Rectangle drawRect = toRayLibRect(tileObject.getDrawingRect());
		auto tilePos = tileObject.getPosition();
		raylib::Rectangle destRec(
			tilePos.x / worldScale + offset.x, 
			tilePos.y / worldScale + offset.y,
			drawRect.width / worldScale, 
			drawRect.height / worldScale);
		texture->Draw(drawRect, destRec, raylib::Vector2(0.0f, 0.0f), 0.0f, tint);
	}
}
	
void TileMap2D::drawImageLayer(
		tson::Layer &layer, const raylib::Camera2D &camera,   
		float worldScale, const raylib::Vector2 &offset, const raylib::Color &tint) {
	auto imageName = layer.getImage();
	auto texIter = textures.find(imageName);
	if(texIter == textures.end()) {
		auto *errMsg = TextFormat("ERROR: Tile-map is missing image: %s\n", 
			imageName.c_str());
		throw std::runtime_error(errMsg);
	}
	auto &texture = texIter->second;
	
	bool repeatX = layer.hasRepeatX();
	bool repeatY = layer.hasRepeatY();
	
	auto screenOriginPos = 
		camera.GetScreenToWorld(raylib::Vector2(0.0f,0.0f));
	auto texWidth = texture->GetWidth();
	auto texHeight = texture->GetHeight();
	raylib::Rectangle sourceRec(
		0.0f, 0.0f, (float)texWidth, (float)texHeight); 
	raylib::Rectangle destRec(
		repeatX ? screenOriginPos.x : offset.x, 
		repeatY ? screenOriginPos.y : offset.y,
		repeatX ? (float)GetScreenWidth() : (float)texWidth, 
		repeatY ? (float)GetScreenHeight() : (float)texHeight);
	raylib::Vector2 origin = raylib::Vector2(0.0f, 0.0f);
	texture->DrawTiled(sourceRec, destRec, origin, 0.0f, 1.0f / worldScale);
}

const raylib::Vector2 TileMap2D::calcLayerOffset(
		tson::Layer &layer, const raylib::Camera2D &camera,   
		float worldScale, const raylib::Vector2 &offset) {
	auto pFactorTson = layer.getParallax();
	auto pOriginTson = layer.getMap()->getParallaxOrigin();
	raylib::Vector2 parallaxFactor(pFactorTson.x, pFactorTson.y);
	raylib::Vector2 parallaxOrigin(pOriginTson.x / worldScale, pOriginTson.y / worldScale);
	
	auto layerOffsetTson = layer.getOffset();
	raylib::Vector2 layerOffset(layerOffsetTson.x, layerOffsetTson.y);
	auto cameraOffset = camera.GetWorldToScreen(parallaxOrigin) - camera.GetOffset();
	
	raylib::Vector2 newOffset = offset + (layerOffset -
		 cameraOffset * (raylib::Vector2(1.0f, 1.0f) - parallaxFactor)) / worldScale;
	
	return newOffset;
}

void TileMap2D::loadImage(const std::string &baseDir, const std::string &fileName) {
	if(textures.find(fileName) != textures.end()) {
		// Already loaded
		return;
	}
	
	std::string fullPath = (baseDir.length() > 0) ?
		TextFormat("%s/%s", baseDir.c_str(), fileName.c_str()) :
		fileName;
	
	textures[fileName] = TextureManager::load(fullPath);
}

