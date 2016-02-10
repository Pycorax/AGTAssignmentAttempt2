#pragma once

// Inheritance Includes
#include "../../StateMachine/Source/State.h"

// Using Directives
using StateMachine::FiniteStateMachine;

class BoomState : public StateMachine::State
{
public:
	BoomState();
	virtual ~BoomState();

	virtual void Init(FiniteStateMachine* FSMOwner);
	virtual void Init(State* stateOwner);
	virtual void Update(double dt);
	virtual void Exit(void);
};