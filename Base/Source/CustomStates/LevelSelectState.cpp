#include "LevelSelectState.h"

#include "../GameStateManager.h"
#include "../gamestate.h"
#include "../CustomScenes/LevelMenuScene.h"

LevelSelectState LevelSelectState::theMenuState;

LevelSelectState* LevelSelectState::Instance()
{
	return &theMenuState;
}

LevelSelectState::LevelSelectState() : CGameState()
{
}

void LevelSelectState::Init(const int width, const int height)
{
	scene = new LevelMenuScene(width, height);
	scene->Init();
}

void LevelSelectState::HandleEvents(CGameStateManager* theGSM)
{

}

void LevelSelectState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{

}

void LevelSelectState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right)
{
	if (button_Left == true)
		scene->UpdateWeaponStatus(scene->WA_FIRE);
}