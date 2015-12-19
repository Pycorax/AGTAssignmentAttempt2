#include "Projectile.h"

/********************************************************************************
 Constructor
 ********************************************************************************/
CProjectile::CProjectile(void)
: m_bActive(false)
, thePosition(0, 0, 0)
, theDirection(0, 0, 0)
, theLength(0.0f)
, theType(PT_DISCRETE)
, speed(0.0f)
{
}

/********************************************************************************
 Destructor
 ********************************************************************************/
CProjectile::~CProjectile(void)
{
}

/********************************************************************************
 Update the status of this projectile
 ********************************************************************************/
void CProjectile::Update(const double dt)
{
	if (m_bActive == false)
		return;

	// Update Position
	thePosition.Set(	thePosition.x + theDirection.x * speed * dt, 
						thePosition.y + theDirection.y * speed * dt, 
						thePosition.z + theDirection.z * speed * dt);
}

/********************************************************************************
 Activate the projectile. true == active, false == inactive
 ********************************************************************************/
void CProjectile::SetActivate(bool m_bActivate)
{
	if (m_bActivate == true)
		m_bActive = true;
	else
		m_bActive = false;
}

/********************************************************************************
 get status of the projectile. true == active, false == inactive
 ********************************************************************************/
bool CProjectile::GetStatus(void)
{
	return m_bActive;
}

/********************************************************************************
 Get the position of the projectile
 ********************************************************************************/
Vector3 CProjectile::GetPosition(void)
{
	return thePosition;
}

/********************************************************************************
 Get the direction of the projectile
 ********************************************************************************/
Vector3 CProjectile::GetDirection(void)
{
	return theDirection;
}

float CProjectile::GetRotationX(void) const
{
	return theRotation.x;
}

float CProjectile::GetRotationY(void) const
{
	return theRotation.y;
}

float CProjectile::GetRotationZ(void) const
{
	return theRotation.z;
}

 float CProjectile::GetLength(void) const
 {
	 return theLength;
 }

 float CProjectile::GetSpeed(void) const
 {
	 return speed;
 }

 int CProjectile::GetType(void) const
 {
	 return theType;
 }

/********************************************************************************
 Set the position of the projectile
 ********************************************************************************/
void CProjectile::SetPosition(Vector3 theNewPosition)
{
	thePosition.Set( theNewPosition.x, theNewPosition.y, theNewPosition.z);
}

/********************************************************************************
 Set the direction of the projectile
 ********************************************************************************/
void CProjectile::SetDirection(Vector3 theNewDirection)
{
	theDirection.Set( theNewDirection.x, theNewDirection.y, theNewDirection.z);

	// Calculate angle of rotation
	theRotation.z = Math::RadianToDegree(acos(theNewDirection.z / sqrt(theNewDirection.x * theNewDirection.x + theNewDirection.y * theNewDirection.y + theNewDirection.z * theNewDirection.z)));
	theRotation.y = Math::RadianToDegree(atan2(theNewDirection.y, theNewDirection.x));
}

void CProjectile::SetLength(float len)
{
	theLength = len;
	theType = PT_RAY;
}

void CProjectile::SetType(int type)
{
	theType = type;
}

/********************************************************************************
 Set speed
 ********************************************************************************/
void CProjectile::SetSpeed(const float speed)
{
	this->speed = speed;
}
