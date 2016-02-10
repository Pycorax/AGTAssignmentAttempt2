#include "MineShrinkState.h"

// State Includes
#include "MineState.h"
#include "BoomState.h"

// Other Include
#include "../Bomber.h"

MineShrinkState::MineShrinkState()
{
}

MineShrinkState::~MineShrinkState()
{
}

void MineShrinkState::Init(FiniteStateMachine * FSMOwner)
{
	State::Init(FSMOwner);
	
	Bomber* bomber = dynamic_cast<Bomber*>(m_FSMOwner->GetParent());

	if (bomber)
	{
		bomber->m_state = Bomber::LS_MINE_SHRINK;
	}
}

void MineShrinkState::Init(State * stateOwner)
{
	State::Init(stateOwner);
}

void MineShrinkState::Update(double dt)
{
	static const float SHRINK_SPEED = -0.3f;
	static const float MAX_SHRINK = -0.8f;
	static const float BOOM_RADIUS = 5.0f;

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
		else
		{
			// Bloat up if near to player
			float bloat = 0;
			if (bomber->m_bloated > MAX_SHRINK)
			{
				bloat = SHRINK_SPEED * dt;
				bomber->m_bloated += bloat;
				bomber->ApplyScale(1 + bloat, 1 + bloat, 1 + bloat);
			}
			else
			{
				// Finish shrinking? Now we're activated!
				changeState(new MineState());
			}
		}
	}
}

void MineShrinkState::Exit(void)
{
	State::Exit();
}
