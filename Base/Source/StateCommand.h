#pragma once

// Inheritance Includes
#include "Command.h"

// Other Includes
#include "GameState.h"

class StateCommand : public Command
{
protected:
	bool m_kill;
	CGameState* m_state;

public:
	StateCommand(CGameState* transitionState, bool kill, string tag = "") : Command(tag)
		, m_state(transitionState)
		, m_kill(kill)
	{
	}

	virtual ~StateCommand()
	{
		
	}

	CGameState* GetState()
	{
		return m_state;
	}

	bool GetKilled()
	{
		return m_kill;
	}
};