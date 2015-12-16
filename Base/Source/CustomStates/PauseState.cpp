#include "PauseState.h"

#include "../GameStateManager.h"
#include "../CustomScenes/PauseMenuScene.h"

PauseState PauseState::theMenuState;

const string PauseState::TAG_RETURN_TO_MENU = "returnToMenu";

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

void PauseState::Update(CGameStateManager * theGSM, const double m_dElapsedTime, string * tagReceiver)
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

}

PauseState* PauseState::Instance()
{
	return &theMenuState;
}

PauseState::PauseState() : CGameState()
{
}