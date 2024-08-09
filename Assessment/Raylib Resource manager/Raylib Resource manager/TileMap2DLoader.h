/** TileMap2DLoader.h
 *
 * Code for loading tile-maps. Separated from TileMap2D, to avoid conflicts
 * with RayLib (clashes with windows.h).
 *
 * @author Hans de Ruiter
 *
 * @copyright (c) 2022 by Kea Sigma Delta Limited, all rights reserved
 *
 * See License.txt for license.
 */

#pragma once

#include "Tileson.h"

#include <string>
#include <memory>

std::unique_ptr<tson::Map> loadTileMap(const std::string &fileName);

 