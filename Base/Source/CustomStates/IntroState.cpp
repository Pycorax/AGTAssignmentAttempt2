#include <iostream>

#include "../GameStateManager.h"
#include "../gamestate.h"
#include "menustate.h"
#include "introstate.h"
#include "playstate.h"
#include "../CustomScenes/SplashScene.h"

CIntroState CIntroState::theIntroState;

CIntroState::~CIntroState()
{
}

void CIntroState::Init(const int width, const int height)
{
	scene = new SplashScene(width, height);
	scene->Init();
}

void CIntroState::HandleEvents(CGameStateManager* theGSM)
{

}

void CIntroState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{

}

void CIntroState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
	const int button_Left, const int button_Middle, const int button_Right)
{
}

CIntroState* CIntroState::Instance()
{
	return &theIntroState;
}

CIntroState::CIntroState() : CGameState()
{
}