#include "PauseState.h"

#include "../GameStateManager.h"
#include "../CustomScenes/PauseMenuScene.h"

PauseState PauseState::theMenuState;

PauseState::~PauseState()
{
}

void PauseState::Init(const int width, const int height)
{
	scene = new PauseMenuScene(width, height);
	scene->Init();
}

void PauseState::HandleEvents(CGameStateManager* theGSM)
{

}

void PauseState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
}

void PauseState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right)
{
	if (button_Left == true)
		scene->UpdateWeaponStatus(scene->WA_FIRE);
}

PauseState* PauseState::Instance()
{
	return &theMenuState;
}

PauseState::PauseState() : CGameState()
{
}