#pragma once

// Inheritance Include
#include "../GameState.h"

class LevelSelectState : public CGameState
{
public:
	void Init(const int width = 800, const int height = 600);

	void Update(CGameStateManager* theGSM, const double m_dElapsedTime, string* tagReceiver = nullptr);

	void HandleEvents(CGameStateManager* theGSM);
	void HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status = true);
	void HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right);

	// Singleton
	static LevelSelectState* Instance();

protected:
	LevelSelectState();

private:
	static LevelSelectState theMenuState;
};
