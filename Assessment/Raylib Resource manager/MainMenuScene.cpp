/** MainMenuScene.cpp
 *
 * See header file for details.
 */

#include "MainMenuScene.h"

#include "Common.h"
#include "ScarfyScene.h"
#include "raylib-cpp.hpp"
#include "ResourceManager.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define TITLE_FONT_SIZE 160
#define TITLE_FONT_SPACING 10.0f
#define MENU_FONT_SIZE 40

#define BUTTON_WIDTH 280
#define BUTTON_HEIGHT 70
#define BUTTON_SPACING (BUTTON_HEIGHT + 10)

MainMenuScene::MainMenuScene() {
	startClicked = false;
	quit = false;
}
	
MainMenuScene::~MainMenuScene() {
}

std::shared_ptr<Scene> MainMenuScene::update(float elapsedTime) {
	Scene::update(elapsedTime);
	if(startClicked) {
		return std::make_shared<ScarfyScene>();
	} else {
		return nullptr;
	}
}

void MainMenuScene::loadResources() {
	musicStream = MusicManager::load(DATADIR "Scarfy-menu-music.mp3");
	musicStream->SetVolume(0.5f);
	musicStream->Play();
}
	
void MainMenuScene::freeResources() {
	musicStream.reset();
}

bool MainMenuScene::shouldQuit() {
	return quit;
}

void MainMenuScene::draw() {
	Scene::draw();
	
	int sceneWidth = GetScreenWidth();
	int sceneHeight = GetScreenHeight();
	
	const char *title = "Scarfy";
	Font titleFont = GetFontDefault();
	raylib::Vector2 titleDim = MeasureTextEx(titleFont, title, TITLE_FONT_SIZE, TITLE_FONT_SPACING); 
	
	int titleLeft = (sceneWidth - (int)titleDim.x) / 2;
	int titleTop = sceneHeight / 4 - (int)titleDim.y / 2;
	raylib::Vector2 titlePos((float)titleLeft, (float)titleTop);
	::DrawTextEx(titleFont, title, titlePos, TITLE_FONT_SIZE, TITLE_FONT_SPACING, RED);
	
	int menuLeft = (sceneWidth - BUTTON_WIDTH) / 2;
	int menuTop = sceneHeight / 2;
	float currX = (float)menuLeft;
	float currY = (float)menuTop;
	
	GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
	GuiSetStyle(DEFAULT, TEXT_SIZE, MENU_FONT_SIZE);
	if (GuiButton(Rectangle{ currX, currY, BUTTON_WIDTH, BUTTON_HEIGHT }, "Start Game")) {
		startClicked = true;
	}
	currY += BUTTON_SPACING;
	if (GuiButton(Rectangle{ currX, currY, BUTTON_WIDTH, BUTTON_HEIGHT }, "Quit")) {
		quit = true;
	}
}

void MainMenuScene::doExit() {
	quit = true;
}