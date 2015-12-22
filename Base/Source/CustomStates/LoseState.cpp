#include "LoseState.h"

#include "../GameStateManager.h"
#include "../CustomScenes/LoseMenuScene.h"
#include "../CustomScenes/IScoreMenu.h"

LoseState LoseState::theMenuState;

const string LoseState::TAG_RETURN_TO_MENU = "returnToMenu";
const string LoseState::TAG_RETRY = "retry";

LoseState::~LoseState()
{
}

void LoseState::Init(const int width, const int height)
{
	scene = new LoseMenuScene(width, height);
	scene->Init();
}

void LoseState::HandleEvents(CGameStateManager* theGSM)
{

}

void LoseState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
}

void LoseState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right)
{
	if (button_Left == true)
		scene->UpdateWeaponStatus(scene->WA_FIRE);
}

void LoseState::Update(CGameStateManager * theGSM, const double m_dElapsedTime, string * tagReceiver)
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

void LoseState::SetScore(int score)
{
	IScoreMenu* scoreScene = dynamic_cast<IScoreMenu*>(scene);

	if (scoreScene)
	{
		scoreScene->SetScore(score);
	}
}

LoseState* LoseState::Instance()
{
	return &theMenuState;
}

LoseState::LoseState() : CGameState()
{
}