/** TileMap2DLoader.cpp
 *
 * See header file for details.
 */

// IMPORTANT: Keep this before other includes
#define POCKETLZMA_LZMA_C_DEFINE
#include "pocketlzma.hpp"

#include "TileMap2DLoader.h"

std::unique_ptr<tson::Map> loadTileMap(const std::string &fileName) {
	tson::Tileson tileson;
	
	if(fileName.ends_with(".lzma")) {
		return tileson.parse(fileName, std::make_unique<tson::Lzma>());
	} else {
		return tileson.parse(fileName);
	}
}
