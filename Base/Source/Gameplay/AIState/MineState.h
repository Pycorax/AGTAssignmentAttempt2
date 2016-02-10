#pragma once

// Inheritance Includes
#include "../../StateMachine/Source/State.h"

// Using Directives
using StateMachine::FiniteStateMachine;

class MineState : public StateMachine::State
{
	float m_mineTimer;

public:
	MineState();
	virtual ~MineState();

	virtual void Init(FiniteStateMachine* FSMOwner);
	virtual void Init(State* stateOwner);
	virtual void Update(double dt);
	virtual void Exit(void);
};