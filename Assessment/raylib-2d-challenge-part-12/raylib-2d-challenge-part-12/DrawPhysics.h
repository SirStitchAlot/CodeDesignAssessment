/** DrawPhysics.h
 *
 * Draws the physics objects to screen (for debugging).
 */

#pragma once

#include "box2d/b2_draw.h"
#include "raylib-cpp.hpp"

#include <vector>

/** Draws the physics objects to screen.
 * This can be used to debug physics issues.
 */
class DrawPhysics : public b2Draw {
public:
	/** Creates a new DrawPhysics object.
	 *
	 * @param worldScale the scale factor for converting from world to screen coordinates
	 */
	DrawPhysics(float worldScale);
	
	virtual ~DrawPhysics();
	
	virtual void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color);

	virtual void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color);
	
	virtual void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color);

	virtual void DrawSolidCircle(const b2Vec2 &center, float radius, 
		const b2Vec2 &axis, const b2Color &color);
	
	virtual void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color);
	
	virtual void DrawTransform(const b2Transform &xf);
	
	virtual void DrawPoint(const b2Vec2 &p, float size, const b2Color &color);
	
private:
	/** For performance reasons, we keep a vertex buffer here (eliminates almost all allocation/reallocation overhead).
	 * WARNING: This is *NOT* thread-safe, so call draw commands from one thread at a time.
	 */
	std::vector<raylib::Vector2> dispVertices;

	float worldScale;
};