#pragma once

// Inheritance Include
#include "../GameState.h"

class InstructionState : public CGameState
{
public:
	virtual ~InstructionState();

	void Init(const int width = 800, const int height = 600);

	void HandleEvents(CGameStateManager* theGSM);
	void HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status = true);
	void HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right);

	static InstructionState* Instance();

protected:
	InstructionState();

private:
	static InstructionState theMenuState;
};
