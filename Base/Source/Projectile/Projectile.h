#pragma once

#include "Vector3.h"
#include <stdio.h>

class CProjectile
{
public:
	CProjectile(void);
	~CProjectile(void);

	enum PROJECTILE_TYPE
	{
		PT_DISCRETE,
		PT_RAY,
		PT_TOTAL
	};

private:
	// Boolean flag to indicate if this projectile is active. If not active, then do not compute/update
	bool m_bActive;
public:
	// Update the status of this projectile
	void Update(const double dt);
	// Activate the projectile. true == active, false == inactive
	void SetActivate(bool m_bActivate);
	// get status of the projectile. true == active, false == inactive
	bool GetStatus(void);
	// Get the position of the projectile
	Vector3 GetPosition();
	// Get the direction of the projectile
	Vector3 GetDirection(void);
	// Get the rotation angles of the projectile
	float GetRotationX(void) const;
	float GetRotationY(void) const;
	float GetRotationZ(void) const;
	// Get the length of the projectile
	float GetLength(void) const; 
	// Get the projectile type
	int GetType(void) const;
	// Set the position of the projectile
	void SetPosition(Vector3 theNewPosition);
	// Set the direction of the projectile
	void SetDirection(Vector3 theNewDirection);
	// Set the length of the ray projectile
	void SetLength(float len);
	// Set the type of the projectile
	void SetType(int type);
	// Set speed
	void SetSpeed(const float speed);
private:
	// The position of the projectile
	Vector3 thePosition;
	// The direction of the projectile
	Vector3 theDirection;
	// The rotation of the projectile
	Vector3 theRotation;
	// Length of the ray projectile
	float theLength;
	// Flag to indicate if this is a sweep projectile
	int theType;
	// Speed
	float speed;
};
