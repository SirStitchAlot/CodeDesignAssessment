/** DisplayOverlay.h
 *
 * @author Hans de Ruiter
 *
 * @copyright (c) 2024 by Kea Sigma Delta Limited, all rights reserved
 *
 * See License.txt for license.
 */

#pragma once

class Scene;

/** Base class for display overlays. These can be used to display game information
 * (e.g., health, score, gold, etc.).
 */
class DisplayOverlay {
public:
    DisplayOverlay();

    virtual ~DisplayOverlay();

    /** Draws the overlay.
     * The default implementation displays nothing. 
     */
    virtual void draw(const Scene &scene);
};