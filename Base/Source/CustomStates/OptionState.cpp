#include "OptionState.h"

#include <iostream>

#include "../GameStateManager.h"
#include "playstate.h"
#include "../gamestate.h"
#include "../CustomScenes/OptionMenuScene.h"

OptionState OptionState::theMenuState;

OptionState::~OptionState()
{
}

void OptionState::Init(const int width, const int height)
{
	scene = new OptionMenuScene(width, height);
	scene->Init();
}

void OptionState::HandleEvents(CGameStateManager* theGSM)
{

}

void OptionState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{

}

void OptionState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right)
{
	if (button_Left == true)
		scene->UpdateWeaponStatus(scene->WA_FIRE);
}

OptionState* OptionState::Instance()
{
	return &theMenuState;
}

OptionState::OptionState() : CGameState()
{
}