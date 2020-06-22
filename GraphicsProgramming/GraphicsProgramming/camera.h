#pragma once
#include <math.h>
#include "Vector3.h"
class camera
{
public:
	camera();
	~camera();
	void update();

	//movement functions
	void moveForward(float dt);
	void moveBackward(float dt);
	void moveRight(float dt);
	void moveLeft(float dt);
	void moveUp(float dt);
	void moveDown(float dt);

	//yaw and pitch setters and getters for rotation
	void setYaw(float x, float dt);
	float getYaw() const;

	void setPitch(float x, float dt);
	float getPitch() const;

	//getters
	float getLookX() const;
	float getLookY() const;
	float getLookZ() const;

	float getUpX() const;
	float getUpY() const;
	float getUpZ() const;

	float getPosX() const;
	float getPosY() const;
	float getPosZ() const;

	//setters
	void setLookX(float x);
	void setLookY(float x);
	void setLookZ(float x);

	void setPosX(float x);
	void setPosY(float x);
	void setPosZ(float x);

	void setUpX(float x);
	void setUpY(float x);
	void setUpZ(float x);
private:
	float cosR, cosP, cosY;
	float sinR, sinP, sinY;
	float Yaw = 0, Pitch = 0, Roll = 0;

	//movement speed of the camera
	float speed = 0.5f;
	//vectors that will store the x, y and z values of camera
	Vector3 forward;
	Vector3 up;
	Vector3 right;
	Vector3 lookat;
	Vector3 position;
};

