#include "WinState.h"

#include "../GameStateManager.h"
#include "../CustomScenes/WinMenuScene.h"
#include "../CustomScenes/IScoreMenu.h"

WinState WinState::theMenuState;

const string WinState::TAG_RETURN_TO_MENU = "returnToMenu";
const string WinState::TAG_RETRY = "retry";

WinState::~WinState()
{
}

void WinState::Init(const int width, const int height)
{
	scene = new WinMenuScene(width, height);
	scene->Init();
}

void WinState::HandleEvents(CGameStateManager* theGSM)
{

}

void WinState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
}

void WinState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right)
{
	if (button_Left == true)
		scene->UpdateWeaponStatus(scene->WA_FIRE);
}

void WinState::Update(CGameStateManager * theGSM, const double m_dElapsedTime, string * tagReceiver)
{
	// For storing the tag
	string tag;

	// Run the default update
	CGameState::Update(theGSM, m_dElapsedTime, &tag);

	// Work with the tag
	if (tag == TAG_RETURN_TO_MENU)
	{
		// Since CGameState::Update() already popped this one, let's pop the game state which comes after this
		theGSM->PopState();
	}
	else if (tag == TAG_RETRY)
	{
		// Reset the Previous Level
		theGSM->ResetLastState();
	}
}

void WinState::SetScore(int score)
{
	IScoreMenu* scoreScene = dynamic_cast<IScoreMenu*>(scene);

	if (scoreScene)
	{
		scoreScene->SetScore(score);
	}
}

WinState* WinState::Instance()
{
	return &theMenuState;
}

WinState::WinState() : CGameState()
{
}