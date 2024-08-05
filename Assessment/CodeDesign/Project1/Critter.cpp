#include "Critter.h"


//add a variable for active critters as in sort the array then keep track of the active critters

//notes/todo 
Critter::Critter()
{
	m_position = Vector2{ 0, 0 };
	m_velocity = Vector2{ 0, 0 };
	m_radius = 0;
	m_isAlive = false;
}

Critter::Critter(Critter& critter){

	m_radius = critter.m_radius;
	m_texture = critter.m_texture;
	m_isAlive = critter.m_isAlive;
	m_isDirty = critter.m_isDirty;
	m_position = critter.m_position;
	m_velocity = critter.m_velocity;
}

Critter::~Critter()
{

	UnloadTexture(m_texture);
	m_isAlive = false;
}
/*=================================^^structors^^========================================================*/
void Critter::Init(Vector2 position, Vector2 velocity, float radius, const char* texture)
{
	m_position = position;
	m_velocity = velocity;
	m_radius = radius;
	
	m_texture = LoadTexture(texture);	

	m_isAlive = true;
}

void Critter::Destroy()
{ //this has something to do with my object pooling thing 
	SetX(400);
	SetY(200);
	SetVelocity(Vector2Subtract(GetVelocity(),GetVelocity()));
	m_isAlive = false;

}

void Critter::Update(float dt)
{
	m_position.x += m_velocity.x * dt;
	m_position.y += m_velocity.y * dt;

	m_isDirty = false;
}


void Critter::Draw()
{
	

	DrawTexture(m_texture, m_position.x, m_position.y, WHITE);
}

void Critter::CheckCollisionScreen(int screenHeight, int screenWidth) {

	if (this->GetX() < 0) {
		this->SetX(0);
		this->SetVelocity(Vector2{ -this->GetVelocity().x, this->GetVelocity().y });
	}	
	else if (this->GetX() > screenWidth) {
		this->SetX(screenWidth);
		this->SetVelocity(Vector2{ -this->GetVelocity().x, this->GetVelocity().y });
	}	
	else if (this->GetY() < 0) {
		this->SetY(0);
		this->SetVelocity(Vector2{ this->GetVelocity().x, -this->GetVelocity().y });
	}	
	else if (this->GetY() > screenHeight) {
		this->SetY(screenHeight);
		this->SetVelocity(Vector2{ this->GetVelocity().x, -this->GetVelocity().y });
	}


}

void Critter::Respawn(Vector2 position, Vector2 velocity) {


	m_isAlive = true;

	m_position = position;
	m_velocity = velocity;


}



void Critter::DeactivateCritter( Critter lastActiveCritter){

	Critter temp(*this);
	*this =lastActiveCritter;
	lastActiveCritter = temp;
}

/*==================================^^Methods^^=================================================================*/

void Critter::operator=(Critter rhs)
{
	m_radius = rhs.m_radius;
	m_texture = rhs.m_texture;
	m_isAlive = rhs.m_isAlive;
	m_isDirty = rhs.m_isDirty;
	m_position = rhs.m_position;
	
}