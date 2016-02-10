#include "DeathState.h"

// Other Include
#include "../Bomber.h"

DeathState::DeathState()
{
}

DeathState::~DeathState()
{
}

void DeathState::Init(FiniteStateMachine * FSMOwner)
{
	State::Init(FSMOwner);
	
	Bomber* bomber = dynamic_cast<Bomber*>(m_FSMOwner->GetParent());

	if (bomber)
	{
		bomber->m_state = Bomber::LS_DEATH;
	}
}

void DeathState::Init(State * stateOwner)
{
	State::Init(stateOwner);
}

void DeathState::Update(double dt)
{
	static const float DEATH_ROTATE_SPEED = 20.0f;
	static const float DEATH_MAX_ROTATE = 30.0f;

	State::Update(dt);

	Bomber* bomber = dynamic_cast<Bomber*>(m_FSMOwner->GetParent());

	if (bomber)
	{
		// Do death animation
		if (bomber->m_deathRotated < DEATH_MAX_ROTATE)
		{
			bomber->ApplyRotate(DEATH_ROTATE_SPEED * dt, 1, 0, 0);
			bomber->m_deathRotated += DEATH_ROTATE_SPEED * dt;
		}
		else
		{
			bomber->Deactivate();
		}
	}
}

void DeathState::Exit(void)
{
	State::Exit();
}
