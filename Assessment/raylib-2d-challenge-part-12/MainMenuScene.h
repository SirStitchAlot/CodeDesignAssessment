/** MainMenuScene.h
 *
 * @author Hans de Ruiter
 *
 * @copyright (c) 2022 by Kea Sigma Delta Limited, all rights reserved
 *
 * See License.txt for license.
 */

#pragma once

#include "Scene.h"

/** The game's main menu.
 */
class MainMenuScene : public Scene {
public:
	MainMenuScene();
	
	virtual ~MainMenuScene();
	
	void loadResources() override;
	
	void freeResources() override;
	
	std::shared_ptr<Scene> update(float elapsedTime) override;
	
	bool shouldQuit() override;
	
	void draw() override;
	
	void doExit() override;
	
private:
	bool startClicked;
	bool quit;
};

