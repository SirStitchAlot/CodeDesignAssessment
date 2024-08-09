/** GameOverlay.h
 *
 * @author Hans de Ruiter
 *
 * @copyright (c) 2024 by Kea Sigma Delta Limited, all rights reserved
 *
 * See License.txt for license.
 */

#pragma once

#include "DisplayOverlay.h"

/** Overlay for the game. Display's the player's essential stats.
 */
class GameOverlay : public DisplayOverlay {
public:
    GameOverlay();

    virtual ~GameOverlay();

    void draw(const Scene &scene) override;
};