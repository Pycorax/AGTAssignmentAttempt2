#include "MineState.h"

// State Includes
#include "MineGrowState.h"
#include "BoomState.h"

// Other Include
#include "../Bomber.h"

MineState::MineState()
	: m_mineTimer(0.0f)
{
}

MineState::~MineState()
{
}

void MineState::Init(FiniteStateMachine * FSMOwner)
{
	State::Init(FSMOwner);
	
	Bomber* bomber = dynamic_cast<Bomber*>(m_FSMOwner->GetParent());

	if (bomber)
	{
		bomber->m_state = Bomber::LS_MINE;
	}
}

void MineState::Init(State * stateOwner)
{
	State::Init(stateOwner);
}

void MineState::Update(double dt)
{
	static const float MINE_IDLE_TIME = 5.0f;
	static const float BOOM_RADIUS = 3.0f;

	State::Update(dt);

	m_mineTimer += dt;

	if (m_mineTimer > MINE_IDLE_TIME)
	{
		changeState(new MineGrowState());
	}
	else
	{
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
		}
	}
}

void MineState::Exit(void)
{
	State::Exit();
}
