/** GameOverlay.cpp
 * 
 * See header file for details.
 */

#include "GameOverlay.h"

#include "Actor.h"
#include "Scene.h"

#define GOLDTEXT_X_OFFSET 20
#define GOLDTEXT_Y_OFFSET 20
#define GOLDTEXT_FONT_SIZE 60
#define GOLDTEXT_FONT_SPACING 1.0f
#define GOLDTEXT_COLOUR DARKGRAY

GameOverlay::GameOverlay() {

}

GameOverlay::~GameOverlay() {

}

void GameOverlay::draw(const Scene &scene) {
    std::shared_ptr<const Actor> player = 
        std::dynamic_pointer_cast<const Actor>(scene.getPlayerAvatar());
    
    if(player) {
        Font titleFont = GetFontDefault();

        // Display the player's gold
        const char *goldText = TextFormat("Gold: %d", player->getWalletValue());
        raylib::Vector2 goldTextDim = MeasureTextEx(
            titleFont, goldText, GOLDTEXT_FONT_SIZE, GOLDTEXT_FONT_SPACING); 
        
        int screenWidth = GetScreenWidth();
        raylib::Vector2 goldTextPos(
            (float)(screenWidth - goldTextDim.GetX() - GOLDTEXT_X_OFFSET), 
            (float)GOLDTEXT_Y_OFFSET);
	    ::DrawTextEx(titleFont, goldText, goldTextPos, 
            GOLDTEXT_FONT_SIZE, GOLDTEXT_FONT_SPACING, GOLDTEXT_COLOUR);
    }
}