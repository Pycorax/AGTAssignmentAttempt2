#include "RunAwayState.h"

// State Includes
#include "ChaseState.h"

// Other Include
#include "../Bomber.h"

RunAwayState::RunAwayState()
{
}

RunAwayState::~RunAwayState()
{
}

void RunAwayState::Init(FiniteStateMachine * FSMOwner)
{
	State::Init(FSMOwner);
	
	Bomber* bomber = dynamic_cast<Bomber*>(m_FSMOwner->GetParent());

	if (bomber)
	{
		bomber->m_state = Bomber::LS_CHASE;
	}
}

void RunAwayState::Init(State * stateOwner)
{
	State::Init(stateOwner);
}

void RunAwayState::Update(double dt)
{
	State::Update(dt);

	Bomber* bomber = dynamic_cast<Bomber*>(m_FSMOwner->GetParent());

	if (bomber)
	{
		// Get a direction away from the target
		Vector3 dir = bomber->GetTranslate() - bomber->m_currentTarget;
		// Discard the y. We don't want to compare height.
		dir.y = 0;

		// Calculate dist to target
		if (dir != Vector3::ZERO_VECTOR)
		{
			// Get Direction
			dir.Normalize();
		}
		
		// Get Movement Vector
		Vector3 move = dir * bomber->m_speed * dt;
		// Move towards
		bomber->ApplyTranslate(move.x, move.y, move.z);

		// If bomber can no longer insta-kill us
		if (!bomber->m_targetInvuln)
		{
			// Return back to chasing
			changeState(new ChaseState());
		}
	}
}

void RunAwayState::Exit(void)
{
	State::Exit();
}
