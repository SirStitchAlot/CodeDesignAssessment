/** DrawPhysics.cpp
 *
 * See header file for details.
 */

#include "DrawPhysics.h"

#include "rlgl.h"

#include <cstdio>

#define ALPHA_VAL 0.5f

#define LINE_THICKNESS 3.0f

static inline raylib::Color toRaylib(const b2Color &color, float alpha = 1.0f) {
#define TO_UCHAR(x) ((unsigned char )(255 * (x)))
	return raylib::Color(TO_UCHAR(color.r), TO_UCHAR(color.g), TO_UCHAR(color.b),
		TO_UCHAR(alpha));
}

DrawPhysics::DrawPhysics(float worldScale) {
	this->worldScale = worldScale;
}

DrawPhysics::~DrawPhysics() {
}

void DrawPhysics::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
	rlDisableBackfaceCulling();
	raylib::Color rColor = toRaylib(color);
	
	Vector2 *rVertices = (Vector2*)vertices;
	DrawLineStrip(rVertices, vertexCount, rColor);
	DrawLineEx(rVertices[vertexCount - 1], rVertices[0], LINE_THICKNESS / worldScale, rColor);
}

void DrawPhysics::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
	rlDisableBackfaceCulling();
	raylib::Color rColor = toRaylib(color, ALPHA_VAL);

	// Raylib's Vector2 happens to be identical to b2Vec2 in memory...
	Vector2 *rVertices = (Vector2*)vertices;
	DrawTriangleFan(rVertices, vertexCount, rColor);
	
	DrawPolygon(vertices, vertexCount, color);
}

void DrawPhysics::DrawCircle(const b2Vec2 &center, float radius, const b2Color &color) {
	rlDisableBackfaceCulling();
	raylib::Color rColor = toRaylib(color);
	
	// DrawCircleLines() uses integer coordinates, which isn't accurate with world coordinates, so...
	int numSides = 32;
	DrawPolyLinesEx((const Vector2&)center, numSides, radius, 0.0f, LINE_THICKNESS / worldScale, rColor);
	
}

void DrawPhysics::DrawSolidCircle(const b2Vec2 &center, float radius, 
		const b2Vec2 &axis, const b2Color &color) {
	rlDisableBackfaceCulling();
	raylib::Color rColor = toRaylib(color, ALPHA_VAL);
	::DrawCircleV((const Vector2&)center, radius, rColor);
	
	DrawCircle(center, radius, color);
	
	// Draw a line, so rotation can be seen
	auto lineEnd = center + radius * axis;
	
	DrawSegment(center, lineEnd, color);
}

void DrawPhysics::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) {
	raylib::Color rColor = toRaylib(color);
	DrawLineEx((const Vector2&)p1, (const Vector2&)p2, LINE_THICKNESS / worldScale, rColor);
}

void DrawPhysics::DrawTransform(const b2Transform &xf) {
	float lineLength = 15 / worldScale;
	float lineThickness = LINE_THICKNESS / worldScale;
	Vector2 origin = (const Vector2&)xf.p;
	Vector2 end = (const raylib::Vector2&)xf.q.GetXAxis() * lineLength;
	DrawLineEx(origin, end, lineThickness, RED);
	
	
	end = (const raylib::Vector2&)xf.q.GetYAxis() * lineLength;
	DrawLineEx(origin, end, lineThickness, GREEN);
}

void DrawPhysics::DrawPoint(const b2Vec2 &p, float size, const b2Color &color) {
	raylib::Color rColor = toRaylib(color);
	float radius = 2.0f / worldScale;
	DrawCircleV((const Vector2&)p, radius, rColor);
}
