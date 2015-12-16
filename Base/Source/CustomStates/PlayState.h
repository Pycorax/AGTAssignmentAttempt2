#pragma once

#include "../GameState.h"

class CPlayState : public CGameState
{
public:
	virtual ~CPlayState();

	void Init(const int width = 800, const int height = 600);;

	void HandleEvents(CGameStateManager* theGSM);
	void HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status = true);
	void HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
					  const int button_Left, const int button_Middle, const int button_Right);

	static CPlayState* Instance();

protected:
	CPlayState();

private:
	static CPlayState thePlayState;
	int counter;

	//Declare variables to store the last and current mouse position
	double mouse_last_x, mouse_last_y, mouse_diff_x, mouse_diff_y;
	double camera_yaw, camera_pitch;
};
