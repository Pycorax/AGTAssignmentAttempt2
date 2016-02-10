#include "BoomState.h"

// Other Include
#include "../Bomber.h"

BoomState::BoomState()
{
}

BoomState::~BoomState()
{
}

void BoomState::Init(FiniteStateMachine * FSMOwner)
{
	State::Init(FSMOwner);

	Bomber* bomber = dynamic_cast<Bomber*>(m_FSMOwner->GetParent());

	if (bomber)
	{
		bomber->m_state = Bomber::LS_BOOM;
		bomber->Deactivate();
	}
}

void BoomState::Init(State * stateOwner)
{
	State::Init(stateOwner);
}

void BoomState::Update(double dt)
{
	State::Update(dt);
}

void BoomState::Exit(void)
{
	State::Exit();
}
