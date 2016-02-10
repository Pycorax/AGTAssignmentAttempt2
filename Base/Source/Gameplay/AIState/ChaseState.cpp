#include "ChaseState.h"

// State Includes
#include "BoomState.h"
#include "RunAwayState.h"

// Other Include
#include "../Bomber.h"

ChaseState::ChaseState()
{
}

ChaseState::~ChaseState()
{
}

void ChaseState::Init(FiniteStateMachine * FSMOwner)
{
	State::Init(FSMOwner);
	
	Bomber* bomber = dynamic_cast<Bomber*>(m_FSMOwner->GetParent());

	if (bomber)
	{
		bomber->m_state = Bomber::LS_CHASE;
	}
}

void ChaseState::Init(State * stateOwner)
{
	State::Init(stateOwner);
}

void ChaseState::Update(double dt)
{
	static const float BOOM_RADIUS = 10.0f;
	static const float GROW_RADIUS = 50.0f;
	static const float BLOAT_SPEED = 0.3f;
	static const float MAX_BLOAT = 1.3f;

	State::Update(dt);

	Bomber* bomber = dynamic_cast<Bomber*>(m_FSMOwner->GetParent());

	if (bomber)
	{
		// Get a direction to the target
		Vector3 dir = bomber->m_currentTarget - bomber->GetTranslate();
		// Discard the y. We don't want to compare height.
		dir.y = 0;

		// Calculate dist to target
		float distToTargetSquared = dir.LengthSquared();

		// If close enough, explode
		if (distToTargetSquared < BOOM_RADIUS * BOOM_RADIUS)
		{
			changeState(new BoomState());
		}
		else // Else Go towards
		{
			// Get Direction
			dir.Normalize();
			// Get Movement Vector
			Vector3 move = dir * bomber->m_speed * dt;
			// Move towards
			bomber->ApplyTranslate(move.x, move.y, move.z);
		}

		// Bloat up if near to player
		float bloat = 0;
		if (distToTargetSquared < GROW_RADIUS * GROW_RADIUS && bomber->m_bloated < MAX_BLOAT)
		{
			bloat = BLOAT_SPEED * dt;
			bomber->m_bloated += bloat;
			bomber->ApplyScale(1 + bloat, 1 + bloat, 1 + bloat);
		}
		else if (bomber->m_bloated > 0.0f)
		{
			bloat = BLOAT_SPEED * dt;
			bomber->m_bloated -= bloat;
			bomber->ApplyScale(1 - bloat, 1 - bloat, 1 - bloat);
		}

		// If the player can insta-kill us
		if (bomber->m_targetInvuln)
		{
			// Runnnnnn
			changeState(new RunAwayState());
		}
	}
}

void ChaseState::Exit(void)
{
	State::Exit();
}
