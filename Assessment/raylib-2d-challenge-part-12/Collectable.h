/** Collectable.h
 *
 * @author Hans de Ruiter
 *
 * @copyright (c) 2023 by Kea Sigma Delta Limited, all rights reserved
 *
 * See License.txt for license.
 */

#pragma once

#include "Actor.h"
#include "PhysicsObject.h"

#include <memory>

#include "TextureAtlas.h"

typedef enum CollectableState {
    CS_AVAILABLE, // Available in the world
    CS_COLLECTING, // Being collected
    CS_COLLECTED // Collected
} CollectableState;

/** Represents a collectable object.
 * A collectable is an item that the player can collect (e.g., coins).
 * 
 * Collectables have the following possible animations:
 * - default - when the collectable is in the world
 * - collect - played when a player "collects" (i.e., picks up or takes) the item
 * 
 * To make a collectable, add at least a default animation to a texture atlas. It must
 * have at least one frame. Ensure that the object's origin is in its centre.
 */
class Collectable : public Actor {
public:
    /** Constructs a new collectable object.
     * 
     * @param typeName the type of colletable
     * @param atlasFileName the name of the atlas file containing the item's animation. It should 
     * contain a sprie/animation wit the name given by typeName
     * @param collectSoundFileName filename of the sound to play when the item is collected
     * @param value the value of this collectable object
     */
    Collectable(const char *typeName, const char *atlasFileName, 
        const char *collectSoundFileName = NULL, int value = 1);

    virtual ~Collectable();

    /** Called when another object collects this item.
     *
     * @return int the value of this item, or 0 if it was already collected
     */
    int collect();

    bool update(float elapsedTime) override;

    void draw(float worldScale) override;
    
    void createPhysicsBody(b2World &world, float worldScale, 
		const b2Vec2 &position, const b2Vec2 &velocity) override;

    void die() override;


protected:
    CollectableState state;

    std::shared_ptr<TextureAtlas> textureAtlas;
    const TASprite *sprite;

    std::unique_ptr<TASpriteAnimationPlayer> defaultAnimPlayer;
    std::unique_ptr<TASpriteAnimationPlayer> collectAnimPlayer;

    std::shared_ptr<raylib::Sound> collectSound;

    PhysicsObject physicsObject;

    int itemValue;
};
