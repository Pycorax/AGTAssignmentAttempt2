#include "InstructionState.h"

#include "../GameStateManager.h"
#include "playstate.h"
#include "../gamestate.h"
#include "../CustomScenes/InstructionMenuScene.h"

InstructionState InstructionState::theMenuState;

InstructionState* InstructionState::Instance()
{
	return &theMenuState;
}

InstructionState::~InstructionState()
{
}

void InstructionState::Init(const int width, const int height)
{
	scene = new InstructionMenuScene(width, height);
	scene->Init();
}

void InstructionState::Pause()
{

}

void InstructionState::Resume()
{

}

void InstructionState::HandleEvents(CGameStateManager* theGSM)
{

}

void InstructionState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{

}

void InstructionState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right)
{
	if (button_Left == true)
		scene->UpdateWeaponStatus(scene->WA_FIRE);
}

InstructionState::InstructionState() : CGameState()
{
}