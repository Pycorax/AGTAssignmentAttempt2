#include "HighScoreState.h"

#include "../GameStateManager.h"
#include "playstate.h"
#include "../gamestate.h"
#include "../CustomScenes/HighScoreMenuScene.h"

HighScoreState HighScoreState::theMenuState;

HighScoreState* HighScoreState::Instance()
{
	return &theMenuState;
}

HighScoreState::HighScoreState() : CGameState()
{
}

void HighScoreState::Init(const int width, const int height)
{
	scene = new HighScoreMenuScene(width, height);
	scene->Init();
}

void HighScoreState::Pause()
{

}

void HighScoreState::Resume()
{

}

void HighScoreState::HandleEvents(CGameStateManager* theGSM)
{

}

void HighScoreState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{

}

void HighScoreState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right)
{
	if (button_Left == true)
		scene->UpdateWeaponStatus(scene->WA_FIRE);
}