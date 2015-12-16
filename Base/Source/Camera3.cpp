#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

// The default camera speed
static const float CAMERA_SPEED = 200.f;

/********************************************************************************
 Constructor
 ********************************************************************************/
Camera3::Camera3()
	: MoveVel_W(0)
	, MoveVel_S(0)
	, MoveVel_A(0)
	, MoveVel_D(0)
	, CAMERA_ACCEL(0)
{
}

/********************************************************************************
 Destructor
 ********************************************************************************/
Camera3::~Camera3()
{
}

/********************************************************************************
 Initialise the camera
 ********************************************************************************/
void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up, Vector3 camOffset)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();

	// Initialise the camera movement flags
	for(int i=0; i<255; i++){
		myKeys[i] = false;
	}

	// Initialise the camera type
	sCameraType = LAND_CAM;

	// For Jump use
	m_bJumping		= false;
	JumpVel			= 0.0f;
	JUMPMAXSPEED	= 10.0f;
	JUMPACCEL		= 10.0f;
	GRAVITY			= -30.0f;

	// Maximum movement speed
	CAMERA_ACCEL = 10.0f;

	// For camera offsetting
	m_camOffset = camOffset;
}

/********************************************************************************
 Update the camera
 ********************************************************************************/
void Camera3::Update(double dt)
{
	// WASD movement
	//if ( myKeys['w'] == true)
	//{
	//	Walk( dt );
	//}
	//else
	//{
	//	MoveVel_W = 0.0f;
	//}
	//if (myKeys['s'] == true)
	//{
	//	Walk( -dt );
	//}
	//else
	//{
	//	MoveVel_S = 0.0f;
	//}
	//if (myKeys['a'] == true)
	//{
	//	Strafe( -dt );
	//}
	//else
	//{
	//	MoveVel_A = 0.0f;
	//}
	//if (myKeys['d'] == true)
	//{
	//	Strafe( dt );
	//}
	//else
	//{
	//	MoveVel_D = 0.0f;
	//}

	// Rotation
	if ( myKeys[VK_UP] == true)
	{
		LookUp( dt );
	}
	if (myKeys[VK_DOWN] == true)
	{
		LookUp( -dt );
	}
	if (myKeys[VK_LEFT] == true)
	{
		TurnLeft( -dt );
	}
	if (myKeys[VK_RIGHT] == true)
	{
		TurnRight( dt );
	}

	// Jump
	/*if (myKeys[32] == true)
	{
		Jump( dt );
		myKeys[32]	= false;
	}
	*/

	UpdateJump(dt);

	//Update the camera direction based on mouse move
	// left-right rotate
	if ( Application::camera_yaw != 0 )
		Yaw( dt );
	if ( Application::camera_pitch != 0 )
		Pitch( dt );

	if(Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

/********************************************************************************
 Update the camera for Third Person View
 Vector3 newPosition is the new position which the camera is to be based on
 ********************************************************************************/
void Camera3::UpdatePosition(Vector3 newPosition, Vector3 newDirection)
{
	// Get the direction to the left so that we can move that way
	Vector3 left = newDirection.Cross(up);
	left.y = 0.0f;
	left.Normalize();

	// Calculate the new position and target
	position = newPosition - (m_camOffset.x * left) - (m_camOffset.z * newDirection) + m_camOffset.y * up;
	target = position + newDirection;
}

/********************************************************************************
 Update the camera status
 ********************************************************************************/
void Camera3::UpdateStatus(const unsigned char key, const bool status)
{
	myKeys[key] = status;
}

/********************************************************************************
 Reset the camera to default settings
 ********************************************************************************/
void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

/********************************************************************************
 Move the camera forward
 ********************************************************************************/
void Camera3::MoveForward(const double dt)
{
	// Calculate the direction vector of the camera
	Vector3 view = (target - position).Normalized();

	// Constrain the movement to the ground if the camera type is land based
	if (sCameraType == LAND_CAM)
	{
		view.y = 0.0f;
		view = view.Normalized();
	}

	MoveVel_W = MoveVel_W + CAMERA_ACCEL * (float)dt;
	if (MoveVel_W > CAMERA_SPEED)
		MoveVel_W = CAMERA_SPEED;

	// Update the camera and target position
	position += view * MoveVel_W;
	target += view * MoveVel_W;
}
/********************************************************************************
 Move the camera backward
 ********************************************************************************/
void Camera3::MoveBackward(const double dt)
{
	Vector3 view = (target - position).Normalized();

	// Constrain the movement to the ground if the camera type is land based
	if (sCameraType == LAND_CAM)
	{
		view.y = 0.0f;
		view = view.Normalized();
	}

	MoveVel_S = MoveVel_S + CAMERA_ACCEL * (float)dt;
	if (MoveVel_S < -CAMERA_SPEED)
		MoveVel_S = -CAMERA_SPEED;

	// Update the camera and target position
	position -= view * MoveVel_S;
	target -= view * MoveVel_S;

}
/********************************************************************************
 Move the camera left
 ********************************************************************************/
void Camera3::MoveLeft(const double dt)
{
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();

	MoveVel_A = MoveVel_A + CAMERA_ACCEL * (float)dt;
	if (MoveVel_A < -CAMERA_SPEED)
		MoveVel_A = -CAMERA_SPEED;

	// Update the camera and target position
	position -= right * MoveVel_A;
	target -= right * MoveVel_A;
}
/********************************************************************************
 Move the camera right
 ********************************************************************************/
void Camera3::MoveRight(const double dt)
{
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();

	MoveVel_D = MoveVel_D + CAMERA_ACCEL * (float)dt;
	if (MoveVel_D > CAMERA_SPEED)
		MoveVel_D = CAMERA_SPEED;

	// Update the camera and target position
	position += right * MoveVel_D;
	target += right * MoveVel_D;
}

/********************************************************************************
 Turn left
 ********************************************************************************/
void Camera3::TurnLeft(const double dt)
{
	Vector3 view = (target - position).Normalized();
	//float yaw = (float)(-CAMERA_SPEED * Application::camera_yaw * (float)dt);
	float yaw = (float)(-CAMERA_SPEED * (float)dt);
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	view = rotation * view;
	target = position + view;
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
}
/********************************************************************************
 Turn right
 ********************************************************************************/
void Camera3::TurnRight(const double dt)
{
	Vector3 view = (target - position).Normalized();
//	float yaw = (float)(-CAMERA_SPEED * Application::camera_yaw * (float)dt);
	float yaw = (float)(-CAMERA_SPEED * (float)dt);
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	view = rotation * view;
	target = position + view;
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
}
/********************************************************************************
 LookUp
 ********************************************************************************/
void Camera3::LookUp(const double dt)
{
	//float pitch = (float)(-CAMERA_SPEED * Application::camera_pitch * (float)dt);
	float pitch = (float)(CAMERA_SPEED * (float)dt);
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	view = rotation * view;
	target = position + view;
}
/********************************************************************************
 LookDown
 ********************************************************************************/
void Camera3::LookDown(const double dt)
{
	//float pitch = (float)(-CAMERA_SPEED * Application::camera_pitch * (float)dt);
	float pitch = (float)(CAMERA_SPEED * (float)dt);
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	view = rotation * view;
	target = position + view;
}
/********************************************************************************
 Spin Clockwise
 ********************************************************************************/
void Camera3::SpinClockWise(const double dt)
{
}
/********************************************************************************
 Spin CounterClockwise
 ********************************************************************************/
void Camera3::SpinCounterClockWise(const double dt)
{
}
/********************************************************************************
 Set Camera Type
 ********************************************************************************/
void Camera3::SetCameraType(CAM_TYPE sCameraType)
{
	this->sCameraType = sCameraType;
}
/********************************************************************************
 Get Camera Type
 ********************************************************************************/
Camera3::CAM_TYPE Camera3::GetCameraType(void)
{
	return sCameraType;
}

/********************************************************************************
 Pitch. You can add in a deadzone here.
 ********************************************************************************/
void Camera3::Pitch(const double dt)
{
	if ( Application::camera_pitch > 0.0 )
		LookUp( -Application::camera_pitch * dt );
	else if ( Application::camera_pitch < 0.0 )
		LookDown( -Application::camera_pitch * dt );
}
/********************************************************************************
 Yaw. You can add in a deadzone here.
 ********************************************************************************/
void Camera3::Yaw(const double dt)
{
	if ( Application::camera_yaw > 0.0 )
		TurnRight( Application::camera_yaw * dt );
	else if ( Application::camera_yaw < 0.0 )
		TurnLeft( Application::camera_yaw * dt );
}
/********************************************************************************
 Roll your camera
 ********************************************************************************/
void Camera3::Roll(const double dt)
{
}
/********************************************************************************
 Walk forward or backward. You can add in a deadzone here.
 ********************************************************************************/
void Camera3::Walk(const double dt)
{
	if (dt > 0)
		MoveForward(dt);
	else if (dt < 0)
		MoveBackward(abs(dt));
}
/********************************************************************************
 Strafe left or right. You can add in a deadzone here.
 ********************************************************************************/
void Camera3::Strafe(const double dt)
{
	if (dt > 0)
		MoveRight(dt);
	else if (dt < 0)
		MoveLeft(abs(dt));
}
/********************************************************************************
 Jump
 ********************************************************************************/
void Camera3::Jump(const double dt)
{
	if (m_bJumping == false)
	{
		m_bJumping = true;

		// Calculate the jump velocity
		JumpVel = JUMPACCEL;// * dt;

		// Factor in maximum speed limit
		if (JumpVel > JUMPMAXSPEED)
			JumpVel = JUMPMAXSPEED;
	}
}

/********************************************************************************
 Update Jump
 ********************************************************************************/
void Camera3::UpdateJump(const double dt)
{
	if (m_bJumping == true)
	{
		// Factor in gravity
		JumpVel += GRAVITY * dt;

		// Update the camera and target position
		position.y += JumpVel * (float)dt;
		target.y += JumpVel * (float)dt;

		// Check if the camera has reached the ground
		if (position.y <= 0)
		{
			position.y = 0;
			target.y = 0;
			JumpVel = 0.0f;
			m_bJumping = false;
		}
	}
}