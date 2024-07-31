#include "Critter.h"


//add a variable for active critters as in sort the array then keep track of the active critters

//notes/todo 
Critter::Critter()
{
	m_position = Vector2{ 0, 0 };
	m_velocity = Vector2{ 0, 0 };
	m_radius = 0;
	m_isLoaded = false;
}

Critter::~Critter()
{
	UnloadTexture(m_texture);
	m_isLoaded = false;
}
/*=================================^^structors^^========================================================*/
void Critter::Init(Vector2 position, Vector2 velocity, float radius, const char* texture)
{
	m_position = position;
	m_velocity = velocity;
	m_radius = radius;
	
	m_texture = LoadTexture(texture);	

	m_isLoaded = true;
}

void Critter::Destroy()
{ //this has something to do with my object pooling thing 
	UnloadTexture(m_texture);
	m_isLoaded = false;
}

void Critter::Update(float dt)
{
	if (m_isLoaded == false)
		return;

	m_position.x += m_velocity.x * dt;
	m_position.y += m_velocity.y * dt;

	m_isDirty = false;
}


void Critter::Draw()
{
	if (m_isLoaded == false)
		return;

	DrawTexture(m_texture, m_position.x, m_position.y, WHITE);
}

void Critter::CheckCollision(int screenHeight, int screenWidth) {

	if (this->GetX() < 0) {
		this->SetX(0);
		this->SetVelocity(Vector2{ -this->GetVelocity().x, this->GetVelocity().y });
	}	
	if (this->GetX() > screenWidth) {
		this->SetX(screenWidth);
		this->SetVelocity(Vector2{ -this->GetVelocity().x, this->GetVelocity().y });
	}	
	if (this->GetY() < 0) {
		this->SetY(0);
		this->SetVelocity(Vector2{ this->GetVelocity().x, -this->GetVelocity().y });
	}	
	if (this->GetY() > screenHeight) {
		this->SetY(screenHeight);
		this->SetVelocity(Vector2{ this->GetVelocity().x, -this->GetVelocity().y });
	}


}
/*==================================^^Methods^^=================================================================*/