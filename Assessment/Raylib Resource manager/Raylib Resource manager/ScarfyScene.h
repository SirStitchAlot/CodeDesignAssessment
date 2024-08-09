/** ScarfyScene.h
 *
 * @author Hans de Ruiter
 *
 * @copyright (c) 2022 by Kea Sigma Delta Limited, all rights reserved
 *
 * See License.txt for license.
 */

#pragma once

#include "TileMap2DScene.h"
#include "Scarfy.h"

/** A scene for scarfy to walk around in.
 */
class ScarfyScene : public TileMap2DScene {
public:
	ScarfyScene();
	
	virtual ~ScarfyScene();
	
	void loadResources() override;
	
	void freeResources() override;
};

