#pragma once

#include "GameStateManager.h"

#include "SceneManager.h"

#define GSM_DEBUG_MODE FALSE

class CGameState
{
public:
	virtual ~CGameState()
	{
		
	}

	virtual void Init(const int width = 800, const int height = 600) = 0;
	virtual void Cleanup() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents(CGameStateManager* theGSM) = 0;
	virtual void HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status = true) = 0;
	virtual void HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right) = 0;
	virtual void Update(CGameStateManager* theGSM) = 0;
	virtual void Update(CGameStateManager* theGSM, const double m_dElapsedTime) = 0;
	virtual void Draw(CGameStateManager* theGSM) = 0;

	void ChangeState(CGameStateManager* theGSM, CGameState* state) {
		theGSM->ChangeState(state);
	}

protected:
	CGameState() { }

	CSceneManager* scene;
};