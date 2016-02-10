#include "MineGrowState.h"

// State Includes
#include "RunAwayState.h"
#include "BoomState.h"

// Other Include
#include "../Bomber.h"

MineGrowState::MineGrowState()
{
}

MineGrowState::~MineGrowState()
{
}

void MineGrowState::Init(FiniteStateMachine * FSMOwner)
{
	State::Init(FSMOwner);
	
	Bomber* bomber = dynamic_cast<Bomber*>(m_FSMOwner->GetParent());

	if (bomber)
	{
		bomber->m_state = Bomber::LS_MINE_UNSHRINK;
	}
}

void MineGrowState::Init(State * stateOwner)
{
	State::Init(stateOwner);
}

void MineGrowState::Update(double dt)
{
	static const float GROW_SPEED = 0.3f;
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
			// Grow back to normal size
			float bloat = 0;
			if (bomber->m_bloated < 0.0f)
			{
				bloat = GROW_SPEED * dt;
				bomber->m_bloated += bloat;
				bomber->ApplyScale(1 + bloat, 1 + bloat, 1 + bloat);
			}
			else
			{
				// Finish growing? Now we're back up!
				changeState(new RunAwayState());
			}
		}
	}
}

void MineGrowState::Exit(void)
{
	State::Exit();
}
