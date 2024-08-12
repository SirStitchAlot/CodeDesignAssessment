#pragma once


#include "raylib.h"
#include "raymath.h"
#include "Resource.h"

class Critter
{
protected:
	Vector2 m_position;
	Vector2 m_velocity;
	float m_radius;

	

	bool m_isAlive;    
						
	bool m_isDirty;		// indicates if we've already processed a collision response for this critter
	
/*===================================================^^Properties^^==================================================*/
public:
	Critter();

	Critter(Critter &critter);

	~Critter();
/*===============================================^^Structors^^==========================================================*/
	
	void Init(Vector2 position, Vector2 velocity, float radius); //this will be used fro critters to stop them from loading so many textures
	void Destroy();
	void Update(float dt);
	void Draw(Resource texture);
	void CheckCollisionScreen(int screenHeight, int screenWidth);
	void Respawn(Vector2 position, Vector2 velocity);
/*================================================^^Method declarations^^============================================================*/
	float GetX() { return m_position.x; }
	float GetY() { return m_position.y; }
	void SetX(float x) { m_position.x = x; }
	void SetY(float y) { m_position.y = y; }

	Vector2 GetPosition() { return m_position; }
	void SetPosition(Vector2 position) { m_position = position; }

	Vector2 GetVelocity() { return m_velocity; }
	void SetVelocity(Vector2 velocity) { m_velocity = velocity; }

	float GetRadius() { return m_radius; }

	bool IsDirty() { return m_isDirty; }
	void SetDirty() { m_isDirty = true; }

	bool IsDead() { return m_isAlive == false; } //we will stop unloading the texture so should i change this? 
	void SetDead() {  m_isAlive = false; }

/*==========================================^^method definations^^================================================================*/




	
};

