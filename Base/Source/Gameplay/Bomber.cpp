#include "Bomber.h"

// STL Includes
#include <sstream>

// State Includes
#include "AIState\ChaseState.h"
#include "AIState\DeathState.h"
#include "AIState\MineShrinkState.h"
#include "AIState\RunAwayState.h"
#include "AIState\FlockState.h"

// Using Directives
using std::ostringstream;



Bomber::Bomber() : CSceneNode(), LuaSerializable("BomberData")
	, m_hat(nullptr)
	, m_head(nullptr)
	, m_body(nullptr)
	, m_speed(10.0f)
	, m_state(LS_CHASE)
	, m_deathRotated(0.0f)
	, m_bloated(0.0f)
	, m_targetInvuln(false)
{
	ConcurrentStateMachine::Init(CI_TOTAL);
	setCurrentState(new ChaseState(), CI_AI);
	setCurrentState(new FlockState(), CI_FLOCK);
}

Bomber::~Bomber()
{
}

void Bomber::Init(Vector3 startPos, Mesh* hatMesh, Mesh* headMesh, Mesh* bodyMesh)
{
	CTransform* rootPos = new CTransform(startPos.x, startPos.y, startPos.z);

	init(rootPos, hatMesh, headMesh, bodyMesh);
}

void Bomber::LoadedInit(LuaFile * L, int id, Mesh * hatMesh, Mesh * headMesh, Mesh * bodyMesh)
{
	// State Restoration
	m_state = static_cast<LIFE_STATE>(static_cast<int>(L->GetNumber(getPropString("State", id))));
	m_deathRotated = L->GetNumber(getPropString("DeathRotation", id));
	m_bloated = L->GetNumber(getPropString("Bloat", id));

	// Position Restoration
	CTransform* rootPos = new CTransform(L->GetNumber(getPropString("PositionX", id)), L->GetNumber(getPropString("PositionY", id)), L->GetNumber(getPropString("PositionZ", id)));

	init(rootPos, hatMesh, headMesh, bodyMesh);
}

bool Bomber::Update(double dt, Vector3 target, bool playerInvuln)
{
	// Do nothing if active is not set to true
	if (GetActive() == false)
	{
		return false;
	}

	// Set the current target
	m_currentTarget = target;

	// Set target invuln status
	m_targetInvuln = playerInvuln;

	// Update the CSM
	ConcurrentStateMachine::Update(dt);

	return m_state == LS_BOOM;
}

void Bomber::Spawn(Vector3 startPos, float speed)
{
	SetTranslate(startPos.x, startPos.y, startPos.z);
	m_speed = speed;
	setCurrentState(new ChaseState(), CI_AI);
	Activate();
}

void Bomber::Kill(void)
{
	m_deathRotated = 0.0f;
	setCurrentState(new DeathState(), CI_AI);
}

void Bomber::SetBodyLODModels(Mesh * lowRes, Mesh * medRes, Mesh * highRes)
{
	m_body->theModel->SetLODMesh(lowRes, medRes, highRes);
}

void Bomber::Nudge(Vector3 direction)
{
	if (direction != Vector3::ZERO_VECTOR)
	{
		// Discard the y
		direction.y = 0.0f;

		// Get the direction unit vector
		direction.Normalize();

		// Get the actual distance to move
		Vector3 move = direction * 5.0f;

		// Move in that direction
		ApplyTranslate(move.x, move.y, move.z);
	}
}

Bomber * Bomber::GetParent(void) const
{
	return m_body;
}

void Bomber::SetFlock(vector<Bomber*>* flock)
{
	m_flock = flock;
}

Bomber::LIFE_STATE Bomber::GetState(void) const
{
	return m_state;
}

string Bomber::SaveStatus(int id)
{
	ostringstream luaScript;

	float x, y, z;
	theTransform->GetOffset(x, y, z);

	// Position
	luaScript << buildPropString("PositionX", to_string(x), id);
	luaScript << buildPropString("PositionY", to_string(y), id);
	luaScript << buildPropString("PositionZ", to_string(z), id);

	// State
	luaScript << buildPropString("State", to_string(m_state), id);
	luaScript << buildPropString("DeathRotation", to_string(m_deathRotated), id);
	luaScript << buildPropString("Bloat", to_string(m_bloated), id);

	return luaScript.str();
}

void Bomber::LoadStatus(LuaFile * L, int id)
{
	// Position
	float x, y, z;
	theTransform->GetOffset(x, y, z);
	ApplyTranslate(-x, -y, -z);
	ApplyTranslate(L->GetNumber(getPropString("PositionX", id)),
								L->GetNumber(getPropString("PositionY", id)),
								L->GetNumber(getPropString("PositionZ", id)));
	
	// State
	m_state = static_cast<LIFE_STATE>(static_cast<int>(L->GetNumber(getPropString("State", id))));
	m_deathRotated = L->GetNumber(getPropString("DeathRotation", id));
	m_bloated = L->GetNumber(getPropString("Bloat", id));

	// Load State (Other states are ignored as they are when active == false
	switch (m_state)
	{
		case LS_CHASE:
			setCurrentState(new ChaseState(), CI_AI);
			break;
		case LS_MINE:
		case LS_MINE_SHRINK:
			setCurrentState(new MineShrinkState(), CI_AI);
			break;
		case LS_RUNAWAY:
		case LS_MINE_UNSHRINK:
			setCurrentState(new RunAwayState(), CI_AI);
			break;
	}
}

void Bomber::init(CTransform * tf, Mesh * hatMesh, Mesh * headMesh, Mesh * bodyMesh)
{
	SetNode(tf, new CModel(bodyMesh));
	m_body = this;
	m_body->ApplyScale(8.0f, 8.0f, 8.0f);
	m_body->SetType(CSceneNode::NT_BOMBER);

	// Head
	CTransform* headPos = new CTransform(0.0f, 1.0f, 0.0f);
	m_head = new Bomber();
	m_head->SetNode(headPos, new CModel(headMesh));
	m_head->ApplyScale(0.4f, 0.4f, 0.4f);
	m_head->SetType(CSceneNode::NT_BOMBER);
	// Let the head know that THIS is the body
	m_head->m_body = this;
	// Add this to be the body
	this->AddChild(m_head);

	// Hat
	CTransform* hatPos = new CTransform(0.0f, 0.8f, 0.0f);
	m_hat = new Bomber();
	m_hat->SetNode(hatPos, new CModel(hatMesh));
	m_hat->ApplyScale(1.5f, 1.5f, 1.5f);
	m_hat->SetType(CSceneNode::NT_BOMBER);
	// Let the hat know that THIS is the body
	m_hat->m_body = this;
	// Add this to be the body
	m_head->AddChild(m_hat);
}
