#pragma once

// Inheritance Include
#include "../GameState.h"
#include "IScoreState.h"

class WinState : public CGameState, public IScoreState
{
public:
	static const string TAG_RETURN_TO_MENU;
	static const string TAG_RETRY;

	virtual ~WinState();

	void Init(const int width = 800, const int height = 600);

	void HandleEvents(CGameStateManager* theGSM);
	void HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status = true);
	void HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right);

	void Update(CGameStateManager* theGSM, const double m_dElapsedTime, string* tagReceiver = nullptr);

	void SetScore(int score);

	static WinState* Instance();

protected:
	WinState();

private:
	static WinState theMenuState;
};
