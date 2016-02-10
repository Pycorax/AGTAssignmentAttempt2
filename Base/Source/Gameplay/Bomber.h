#pragma once

// Inheritance Include
#include "..\SceneGraph\SceneNode.h"
#include "..\LuaSerializable.h"
#include "ConcurrentStateMachine.h"

// Using Directives
using StateMachine::ConcurrentStateMachine;

class Bomber : public CSceneNode, public LuaSerializable, public ConcurrentStateMachine
{
	/*
 	 *		State Classes Friend Declarations
 	 *		States should be able to access this class's properties
	 */
	friend class State;
	friend class ChaseState;
	friend class BoomState;
	friend class DeathState;
	friend class FlockState;
	friend class RunAwayState;

private:
	// Static Constants
	static const float DEATH_ROTATE_SPEED;
	static const float DEATH_MAX_ROTATE;

public:
	enum CSM_ITEM
	{
		CI_AI,
		CI_FLOCK,
		CI_TOTAL
	};

	enum LIFE_STATE
	{
		LS_CHASE,
		LS_BOOM,
		LS_DEATH,
		LS_TOTAL
	};

private:
	// Body Parts
	Bomber* m_hat;
	Bomber* m_head;
	Bomber* m_body;

	// Human Stats
	float m_speed;
	LIFE_STATE m_state;

	// Animation
	float m_deathRotated;		// Angle rotated for the death animation
	float m_bloated;			// Stores the amount of bloat scale applied

	// AI
	// -- Normal AI
	Vector3 m_currentTarget;
	bool m_targetInvuln;
	// -- Flocking AI
	vector<Bomber*>* m_flock;

public:
	Bomber();
	virtual ~Bomber();

	// Life Time
	void Init(Vector3 startPos, Mesh* hatMesh, Mesh* headMesh, Mesh* bodyMesh);		// Should only be called on Body Bomber nodes
	void LoadedInit(LuaFile* L, int id, Mesh* hatMesh, Mesh* headMesh, Mesh* bodyMesh);
	bool Update(double dt, Vector3 target, bool playerInvuln);
	
	void Spawn(Vector3 startPos, float speed);
	void Kill(void);

	// Models
	void SetBodyLODModels(Mesh* lowRes, Mesh* medRes, Mesh* highRes);

	// Interaction
	void Nudge(Vector3 direction);
	Bomber* GetParent(void) const;

	// Others
	void SetFlock(vector<Bomber*>* flock);

	/*
	* Implemented abstract functions for LuaSerializable
	*/
	virtual string SaveStatus(int id = -1);
	virtual void LoadStatus(LuaFile* L, int id = -1);

private:
	void init(CTransform* tf, Mesh * hatMesh, Mesh * headMesh, Mesh * bodyMesh);
};
