/** RayLib 2D Challenge main.
 *
 * @author Hans de Ruiter
 *
 * @copyright (c) 2022 by Kea Sigma Delta Limited, all rights reserved
 *
 * See License.txt for license.
 */

#include "InputHandler.h"
#include "MainMenuScene.h"
#include "raylib-cpp.hpp"

#include <stddef.h>
#include <stdlib.h>

const int screenWidth = 1600;
const int screenHeight = 900;


/** Displays a single line error message, and exits when the user pushes ESCape, or the window's close
 * button.
 */
void showErrorAndExit(const char *errMsg) {
	SetExitKey(KEY_ESCAPE);
	while (!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(BLACK);
			DrawText(errMsg, 20, 20, 20, RED);
		EndDrawing();
	}
	
	exit(EXIT_FAILURE);
}

int main(void)
{
	int retVal = EXIT_SUCCESS;
	
	// Initialization
	// NOTE: These are put outside the try block, so that they still exist in the catch block.
	// Otherwise we'll never see any error messages...
	raylib::Window window(screenWidth, screenHeight, "RayLib - 2D Character Animation");
	raylib::AudioDevice audioDevice(false);
		
	try {
		InputHandler inputHandler;
		
		std::shared_ptr<Scene> currScene = std::make_shared<MainMenuScene>();
		currScene->loadResources();
		currScene->start();

		SetTargetFPS(60);
		SetExitKey(KEY_NULL);
		

		// Main game loop
		bool quit = false;
		while (!quit) {
			// Update
			inputHandler.handleInput(*currScene);
			float elapsedTime = GetFrameTime();
			auto nextScene = currScene->update(elapsedTime);
			if(nextScene) {
				nextScene->loadResources();
				nextScene->start();
				currScene = nextScene;
			}
			quit = WindowShouldClose() || currScene->shouldQuit();
			
			// Draw
			BeginDrawing();
			currScene->draw();
			EndDrawing();
		}
	} catch(std::runtime_error &e) {
		showErrorAndExit(e.what());
		retVal = EXIT_FAILURE;
	} catch(const char *e) {
		showErrorAndExit(e);
		retVal = EXIT_FAILURE;
	} catch(...) {
		showErrorAndExit("An unknown error occurred.");
		retVal = EXIT_FAILURE; 
	}
	
    return retVal;
}
