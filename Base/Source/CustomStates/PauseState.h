#pragma once

// Inheritance Include
#include "../GameState.h"

class PauseState : public CGameState
{
public:
	static const string TAG_RETURN_TO_MENU;

	virtual ~PauseState();

	void Init(const int width = 800, const int height = 600);

	void HandleEvents(CGameStateManager* theGSM);
	void HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status = true);
	void HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right);

	void Update(CGameStateManager* theGSM, const double m_dElapsedTime, string* tagReceiver = nullptr);

	static PauseState* Instance();

protected:
	PauseState();

private:
	static PauseState theMenuState;
};
