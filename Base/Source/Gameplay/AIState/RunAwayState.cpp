#include "RunAwayState.h"

// State Includes
#include "ChaseState.h"
#include "MineShrinkState.h"
#include "BoomState.h"

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
		bomber->m_state = Bomber::LS_RUNAWAY;
	}
}

void RunAwayState::Init(State * stateOwner)
{
	State::Init(stateOwner);
}

void RunAwayState::Update(double dt)
{
	static const float MINE_CHANCE = 0.1f;
	static const float BOOM_RADIUS = 7.5f;

	State::Update(dt);

	Bomber* bomber = dynamic_cast<Bomber*>(m_FSMOwner->GetParent());

	if (bomber)
	{
		// Get a direction away from the target
		Vector3 dir = bomber->GetTranslate() - bomber->m_currentTarget;
		float distToTargetSquared = dir.LengthSquared();
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

		// Do Bounds Checking
		Vector3 finalPos;
		bomber->theTransform->GetOffset(finalPos.x, finalPos.y, finalPos.z);
		finalPos += move;
		if (finalPos.x < bomber->m_minBounds.x || finalPos.x > bomber->m_maxBounds.x)
		{
			move.x = 0.0;
		}
		if (finalPos.z < bomber->m_minBounds.z || finalPos.z > bomber->m_maxBounds.z)
		{
			move.z = 0.0;
		}

		// Move towards
		bomber->ApplyTranslate(move.x, move.y, move.z);

		// If bomber can no longer insta-kill us
		if (!bomber->m_targetInvuln)
		{
			// Return back to chasing
			changeState(new ChaseState());
		}
		// If close enough, explode
		else if (distToTargetSquared < BOOM_RADIUS * BOOM_RADIUS)
		{
			changeState(new BoomState());
		}
		else
		{
			float chance = Math::RandFloatMinMax(0.0f, 100.0f);
			if (chance <= MINE_CHANCE)
			{
				changeState(new MineShrinkState());
			}
		}
	}
}

void RunAwayState::Exit(void)
{
	State::Exit();
}
