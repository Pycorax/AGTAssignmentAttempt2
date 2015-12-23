#include "PlayInfo3PV.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "Mtx44.h"

const float CPlayInfo3PV::DIRECTION_SPEED = 100.0f;
const float CPlayInfo3PV::MAX_PITCH = 60.0f;

CPlayInfo3PV::CPlayInfo3PV(void)
	: theAvatarMesh(NULL)
	, jumpspeed(0)
	, m_rotationY(0.0f)
	, m_pitch(0.0f)
	, curScale(Vector3(5, 8, 5))
	, m_movedForwardOnly(false)
	, m_state(PS_NORMAL)
{
	Init();
}


CPlayInfo3PV::~CPlayInfo3PV(void)
{
}

// Initialise this class instance
void CPlayInfo3PV::Init(void)
{
	curPosition.Set( 0, 0, 0);
	curDirection.Set( 0, 0, 1 );
	curUp.Set(0, 1, 0);

	m_movementSpeed[PS_NORMAL] = 120.0f;
	m_movementSpeed[PS_SPRINT] = 200.0f;

	// Initialise the Avatar's movement flags
	for(int i=0; i<255; i++){
		myKeys[i] = false;
	}
}

// Set Model
bool CPlayInfo3PV::SetModel( Mesh* theAvatarMesh )
{
	this->theAvatarMesh = theAvatarMesh;
	if (this->theAvatarMesh == NULL)
		return false;

	return true;
}

// Returns true if the player is on ground
bool CPlayInfo3PV::isOnGround(void)
{
	return true;
}

// Returns true if the player is jumping upwards
bool CPlayInfo3PV::isJumpUpwards(void)
{
	return true;
}

// Returns true if the player is on freefall
bool CPlayInfo3PV::isFreeFall(void)
{
	return true;
}

// Set the player's status to free fall mode
void CPlayInfo3PV::SetOnFreeFall(bool isOnFreeFall)
{
	if (isOnFreeFall == true)
	{
		jumpspeed = 0;
	}
}

// Set the player to jumping upwards
void CPlayInfo3PV::SetToJumpUpwards(bool isOnJumpUpwards)
{
	if (isOnJumpUpwards == true)
	{
		jumpspeed = 15;
	}
}

// Set position x of the player
void CPlayInfo3PV::SetPos_x(int pos_x)
{
	curPosition.x = pos_x;
}

// Set position y of the player
void CPlayInfo3PV::SetPos_y(int pos_y)
{
	curPosition.y = pos_y;
}

// Set position y of the player
void CPlayInfo3PV::SetPos_z(int pos_z)
{
	curPosition.z = pos_z;
}

// Set Jumpspeed of the player
void CPlayInfo3PV::SetJumpspeed(int jumpspeed)
{
	this->jumpspeed = jumpspeed;
}

// Stop the player's movement
void CPlayInfo3PV::SetToStop(void)
{
	jumpspeed = 0;
}

/********************************************************************************
 Hero Move Up Down
 ********************************************************************************/
void CPlayInfo3PV::MoveFrontBack(const bool mode, const float timeDiff)
{
	if (mode)
	{
		m_deltaMovement.z -= 1.0f;
	}
	else
	{
		m_deltaMovement.z += 1.0f;
	}
}

/********************************************************************************
 Hero Move Left Right
 ********************************************************************************/
void CPlayInfo3PV::MoveLeftRight(const bool mode, const float timeDiff)
{
	if (mode)
	{
		m_deltaMovement.x += 1.0f;
	}
	else
	{
		m_deltaMovement -= 1.0f;
	}
}


// Get position x of the player
float CPlayInfo3PV::GetPos_x(void)
{
	return curPosition.x;
}

// Get position y of the player
float CPlayInfo3PV::GetPos_y(void)
{
	return curPosition.y;
}

// Get position y of the player
float CPlayInfo3PV::GetPos_z(void)
{
	return curPosition.z;
}

// Get position of the player
Vector3 CPlayInfo3PV::GetPosition()
{
	return curPosition;
}

// Get direction of the player
Vector3 CPlayInfo3PV::GetDirection()
{
	return curDirection;
}

Vector3 CPlayInfo3PV::GetUpDir()
{
	return curUp;
}

Vector3 CPlayInfo3PV::GetScale() const
{
	return curScale;
}

float CPlayInfo3PV::GetYRotation(void) const
{
	return m_rotationY;
}

// Get Jumpspeed of the player
float CPlayInfo3PV::GetJumpspeed(void)
{
	return jumpspeed;
}

bool CPlayInfo3PV::GetMovedForwardOnly() const
{
	return m_movedForwardOnly;
}

bool CPlayInfo3PV::IsSprinting(void) const
{
	return m_state == PS_SPRINT;
}

// Update Jump Upwards
void CPlayInfo3PV::UpdateJumpUpwards()
{
	curPosition.y -= jumpspeed;
	jumpspeed -= 1;
	if (jumpspeed == 0)
	{
	}
}

// Update FreeFall
void CPlayInfo3PV::UpdateFreeFall()
{
	curPosition.y += jumpspeed;
	jumpspeed += 1;
}

// Constrain the position of the Hero to within the border
void CPlayInfo3PV::ConstrainHero(const int leftBorder, const int rightBorder, 
								  const int frontBorder, const int backBorder, 
								  float timeDiff)
{
	if (curPosition.x < leftBorder + curScale.x * 0.5f)
	{
		curPosition.x = leftBorder + curScale.x * 0.5f;
	}
	else if (curPosition.x > rightBorder - curScale.x * 0.5f)
	{
		curPosition.x = rightBorder - curScale.x * 0.5f;
	}

	if (curPosition.z < frontBorder + curScale.z * 0.5f)
	{
		curPosition.z = frontBorder + curScale.z * 0.5f;
	}
	else if (curPosition.z > backBorder - curScale.z * 0.5f)
	{
		curPosition.z = backBorder - curScale.z * 0.5f;
	}
}


/********************************************************************************
 Update Movement
 ********************************************************************************/
void CPlayInfo3PV::UpdateMovement(const unsigned char key, const bool status)
{
	myKeys[key] = status;
}

/********************************************************************************
 Update
 ********************************************************************************/
void CPlayInfo3PV::Update(double dt)
{
	// Reset movement flag each update
	m_movedForwardOnly = false;

	// WASD movement
	if ( myKeys['w'] == true)
	{
		// We had moved forwards!
		m_movedForwardOnly = true;

		MoveFrontBack( false, dt );
	}
	if (myKeys['s'] == true)
	{
		m_movedForwardOnly = false;
		MoveFrontBack( true, dt );
	}
	if (myKeys['a'] == true)
	{
		m_movedForwardOnly = false;
		MoveLeftRight( true, dt );
	}
	if (myKeys['d'] == true)
	{
		m_movedForwardOnly = false;
		MoveLeftRight( false, dt );
	}

	if (myKeys[VK_SHIFT] == true)
	{
		if (m_movedForwardOnly)
		{
			m_state = PS_SPRINT;
		}
		else
		{
			// Dash code here
			m_state = PS_NORMAL;
		}
	}
	else
	{
		m_state = PS_NORMAL;
	}

	// Do calculations to move if we can actually move
	if (m_deltaMovement != Vector3::ZERO_VECTOR)
	{
		// Normalize the delta movement
		m_deltaMovement.Normalize();

		// Set default rotation
		Mtx44 charRotMatrix;
		charRotMatrix.SetToRotation(m_rotationY, 0, 1, 0);
		// Get change in position with regards to direction
		Vector3 deltaPos = charRotMatrix * (m_deltaMovement * m_movementSpeed[m_state] * dt);
		// Add the resulting change in position to char position
		curPosition += deltaPos;

		// Reset the Movement
		m_deltaMovement = Vector3::ZERO_VECTOR;
	}

	// Rotation
	if (myKeys[VK_UP] == true)
	{
		LookUp(dt);
	}
	if (myKeys[VK_DOWN] == true)
	{
		LookDown(-dt);
	}
	if (myKeys[VK_LEFT] == true)
	{
		TurnLeft(-dt);
	}
	if (myKeys[VK_RIGHT] == true)
	{
		TurnRight(dt);
	}

	// Jump
	/*if (myKeys[32] == true)
	{
	Jump( dt );
	myKeys[32]	= false;
	}
	*/

	//UpdateJump(dt);

	//Update the camera direction based on mouse move
	// left-right rotate
	if (Application::camera_yaw != 0)
		Yaw(dt);
	if (Application::camera_pitch != 0)
		Pitch(dt);

	// Constrain Player
	
}

/********************************************************************************
Turn left
********************************************************************************/
void CPlayInfo3PV::TurnLeft(const double dt)
{
	Vector3 view = (curDirection).Normalized();
	//float yaw = (float)(-CAMERA_SPEED * Application::camera_yaw * (float)dt);
	float yaw = (float)(-DIRECTION_SPEED * (float)dt);
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	curDirection = rotation * view;
	Vector3 right = view.Cross(curUp);
	right.y = 0;
	right.Normalize();
	curUp = right.Cross(view).Normalized();

	// Update the Rotation Angle
	m_rotationY += yaw;
}
/********************************************************************************
Turn right
********************************************************************************/
void CPlayInfo3PV::TurnRight(const double dt)
{
	Vector3 view = (curDirection).Normalized();
	//	float yaw = (float)(-CAMERA_SPEED * Application::camera_yaw * (float)dt);
	float yaw = (float)(-DIRECTION_SPEED * (float)dt);
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	curDirection = rotation * view;
	Vector3 right = view.Cross(curUp);
	right.y = 0;
	right.Normalize();
	curUp = right.Cross(view).Normalized();

	// Update the Rotation Angle
	m_rotationY += yaw;
}
/********************************************************************************
LookUp
********************************************************************************/
void CPlayInfo3PV::LookUp(const double dt)
{
	float pitch = (float)(DIRECTION_SPEED * (float)dt);

	// Check if the pitch is within pitching limits
	if ((m_pitch + pitch) > MAX_PITCH)
	{
		// Set the pitch to the maximum we can pitch without over-pitching
		pitch = MAX_PITCH - m_pitch;
	}

	Vector3 view = (curDirection).Normalized();
	Vector3 right = view.Cross(curUp);
	right.y = 0;
	right.Normalize();
	curUp = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	curDirection = rotation * view;
	m_pitch += pitch;
}
/********************************************************************************
LookDown
********************************************************************************/
void CPlayInfo3PV::LookDown(const double dt)
{
	float pitch = (float)(DIRECTION_SPEED * (float)dt);

	// Check if the pitch is within pitching limits
	if ((m_pitch + pitch) < -MAX_PITCH)
	{
		// Set the pitch to the maximum we can pitch without over-pitching
		pitch = -MAX_PITCH - m_pitch;
	}

	Vector3 view = (curDirection).Normalized();
	Vector3 right = view.Cross(curUp);
	right.y = 0;
	right.Normalize();
	curUp = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	curDirection = rotation * view;
	m_pitch += pitch;
}

/********************************************************************************
Pitch. You can add in a deadzone here.
********************************************************************************/
void CPlayInfo3PV::Pitch(const double dt)
{
	if (Application::camera_pitch > 0.0)
		LookDown(-Application::camera_pitch * dt);
	else if (Application::camera_pitch < 0.0)
		LookUp(-Application::camera_pitch * dt);
}
/********************************************************************************
Yaw. You can add in a deadzone here.
********************************************************************************/
void CPlayInfo3PV::Yaw(const double dt)
{
	if (Application::camera_yaw > 0.0)
		TurnRight(Application::camera_yaw * dt);
	else if (Application::camera_yaw < 0.0)
		TurnLeft(Application::camera_yaw * dt);
}