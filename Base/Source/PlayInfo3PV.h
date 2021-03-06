#pragma once

// Inheritance Includes
#include "LuaSerializable.h"

// Other Includes
#include "Vector3.h"
#include "Mesh.h"

class CPlayInfo3PV : public LuaSerializable
{
	// Turning speed
	static const float DIRECTION_SPEED;
	// Maximum pitch allowance
	static const float MAX_PITCH;

public:
	CPlayInfo3PV(void);
	virtual ~CPlayInfo3PV(void);

	enum GEOMETRY_TYPE
	{
		GEO_AVATAR,
		NUM_GEOMETRY,
	};

	enum PLAYER_STATE_TYPE
	{
		PS_NORMAL,
		PS_SPRINT,
		PS_TOTAL
	};

	// Initialise this class instance
	void Init(void);

	// Set Model
	bool SetModel( Mesh* theAvatarMesh );

	// Returns true if the player is on ground
	bool isOnGround(void);
	// Returns true if the player is jumping upwards
	bool isJumpUpwards(void);
	// Returns true if the player is on freefall
	bool isFreeFall(void);
	// Set the player's status to free fall mode
	void SetOnFreeFall(bool isOnFreeFall);
	// Set the player to jumping upwards
	void SetToJumpUpwards(bool isOnJumpUpwards);
	// Stop the player's movement
	void SetToStop(void);
	// Set position x of the player
	void SetPos_x(int pos_x);
	// Set position y of the player
	void SetPos_y(int pos_y);
	// Set position z of the player
	void SetPos_z(int pos_y);	
	// Set Jumpspeed of the player
	void SetJumpspeed(int jumpspeed);

	// Update Movements
	void MoveFrontBack(const bool mode, const float timeDiff);
	void MoveLeftRight(const bool mode, const float timeDiff);

	// Get position x of the player
	float GetPos_x(void);
	// Get position y of the player
	float GetPos_y(void);
	// Get position z of the player
	float GetPos_z(void);
	// Get position of the player
	Vector3 GetPosition();
	// Get direction of the player
	Vector3 GetDirection();
	// Get the up direction of the player
	Vector3 GetUpDir();
	// Get Current Scale
	Vector3 GetScale() const;
	// Get Y-rotation of player
	float GetYRotation(void) const;
	// Get Jumpspeed of the player
	float GetJumpspeed(void);
	// Check if the player had moved in the last frame
	bool GetMovedForwardOnly() const;

	/*
	 * Sprinting
	 */
	bool IsSprinting(void) const;

	// Update Jump Upwards
	void UpdateJumpUpwards();
	// Update FreeFall
	void UpdateFreeFall();
	// Update
	void UpdateMovement(const unsigned char key, const bool status = true);
	// Update
	void Update(double dt);

	// Reset the avatar to default settings
	void Reset();

	// Constrain the position of the Hero to within the border
	void ConstrainHero(const int leftBorder, const int rightBorder, 
					   const int frontBorder, const int backBorder,
					   float timeDiff);

	// Avatar's Mesh
	Mesh*	theAvatarMesh;

	void TurnLeft(const double dt);
	void TurnRight(const double dt);
	void LookUp(const double dt);
	void LookDown(const double dt);

	virtual void Pitch(const double dt);
	virtual void Yaw(const double dt);

	/*
	 * Implemented abstract functions for LuaSerializable
	 */
	virtual string SaveStatus(int id = -1);
	virtual void LoadStatus(LuaFile* L, int id = -1);

private:
	// Hero's information
	Vector3 curPosition;
	Vector3 curDirection;
	Vector3 curScale;
	float m_rotationY;
	Vector3 curUp;
	int jumpspeed;

	// Stores the movement direction for a frame. to be cleared each frame
	Vector3 m_deltaMovement;
	// Movement Speed
	float m_movementSpeed[PS_TOTAL];
	// Stores if there was movement in the last frame. Allows other objects to respond to this.
	bool m_movedForwardOnly;

	// The current state of the player
	PLAYER_STATE_TYPE m_state;

	// Stores the current pitching angle for limiting the pitch
	float m_pitch;

	bool myKeys[255];
};

