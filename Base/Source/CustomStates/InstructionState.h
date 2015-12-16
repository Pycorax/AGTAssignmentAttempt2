#pragma once

// Inheritance Include
#include "../GameState.h"

class InstructionState : public CGameState
{
public:
	void Init(const int width = 800, const int height = 600);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(CGameStateManager* theGSM);
	void HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status = true);
	void HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right);
	void Update(CGameStateManager* theGSM);
	void Update(CGameStateManager* theGSM, const double m_dElapsedTime);
	void Draw(CGameStateManager* theGSM);

	static InstructionState* Instance() {
		return &theMenuState;
	}

protected:
	InstructionState() { }

private:
	static InstructionState theMenuState;
};
