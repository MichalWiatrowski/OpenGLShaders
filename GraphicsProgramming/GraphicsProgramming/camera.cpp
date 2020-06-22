#include "camera.h"



camera::camera()
{
	position.x = 0;
	position.y = 5;
	position.z = 15;
	lookat.x = 0;
	lookat.y = -2;
	lookat.z = 0;
	up.x = 0;
	up.y = 1;
	up.z = 0;
}


camera::~camera()
{
}

void camera::update()
{
	//calculate the maths
	cosY = cosf(Yaw*3.1415 / 180);
	cosP = cosf(Pitch*3.1415 / 180);
	cosR = cosf(Roll*3.1415 / 180);
	sinY = sinf(Yaw*3.1415 / 180);
	sinP = sinf(Pitch*3.1415 / 180);
	sinR = sinf(Roll*3.1415 / 180);
	
	//calculates the forward vector

	forward.x = sinY * cosP;
	forward.y = sinP;
	forward.z = cosP * -cosY;

	//calculates the lookat vector
	lookat = forward + position;
	
	//calculates  the up vector
	up.x = -cosY * sinR - sinY * sinP * cosR;
	up.y = cosP * cosR; 
	up.z = -sinY * sinR - sinP * cosR * -cosY;
	
	//calculates the right vector
	right = forward.cross(up);
	
	

}
void camera::moveForward(float dt)
{
	position.x += forward.x * speed * dt;
	position.y += forward.y * speed * dt;
	position.z += forward.z * speed * dt;
}

void camera::moveBackward(float dt)
{
	position.x -= forward.x * speed * dt;
	position.y -= forward.y * speed * dt;
	position.z -= forward.z * speed * dt;

}

void camera::moveRight(float dt)
{
	position.x += right.x * speed *dt;
	position.y += right.y * speed *dt;
	position.z += right.z * speed *dt;
}

void camera::moveLeft(float dt)
{
	position.x -= right.x * speed *dt;
	position.y -= right.y * speed *dt;
	position.z -= right.z * speed *dt;
}

void camera::moveUp(float dt)
{
	position.y += up.y * speed * dt;
}

void camera::moveDown(float dt)
{	
	position.y -= up.y * speed * dt;
}

///////////////////////////////////////////////////////////////////// getters

float camera::getPosX() const
{
	return position.x;
}
float camera::getPosY() const
{
	return position.y;
}

float camera::getPosZ() const
{
	return position.z;
}

float camera::getUpX() const
{
	return up.x;
}

float camera::getUpY() const
{
	return up.y;
}

float camera::getUpZ() const
{
	return up.z;
}


float camera::getPitch() const
{
	return Pitch;
}

float camera::getLookX() const
{
	return lookat.x;
}

float camera::getLookY() const
{
	return lookat.y;
}

float camera::getLookZ() const
{
	return lookat.z;
}
/////////////////////////////////////////////////////////////////// setters
void camera::setLookX(float x)
{
	lookat.x = x;
}
void camera::setLookY(float x)
{
	lookat.y = x;
}
void camera::setLookZ(float x)
{
	lookat.z = x;

}

void camera::setUpX(float x)
{
	up.x = x;
}

void camera::setUpY(float x)
{
	up.y = x;
}

void camera::setUpZ(float x)
{
	up.z = x;
}

void camera::setPosX(float x)
{
	position.x = x;
}

void camera::setPosY(float x)
{
	position.y = x;
}

void camera::setPosZ(float x)
{
	position.z = x;
}

void camera::setYaw(float x, float dt)
{
	cosY = cosf(Yaw*3.1415 / 180);
	sinY = sinf(Yaw*3.1415 / 180);
	Yaw = x * 0.5;
}
float camera::getYaw() const
{
	return Yaw;
}
void camera::setPitch(float x, float dt)
{
	cosP = cosf(Pitch*3.1415 / 180);
	sinP = sinf(Pitch*3.1415 / 180);
	Pitch = -x * 0.5;
}

;