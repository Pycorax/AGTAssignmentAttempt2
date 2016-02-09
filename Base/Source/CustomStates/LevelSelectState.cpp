#include "LevelSelectState.h"

#include "../GameStateManager.h"
#include "../gamestate.h"
#include "../CustomScenes/LevelMenuScene.h"
#include "PlayState.h"

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

void LevelSelectState::Update(CGameStateManager * theGSM, const double m_dElapsedTime, string * tagReceiver)
{
	// Check if there is a scene
	if (scene)
	{
		// Update the scene
		scene->Update(m_dElapsedTime);

		// Retrieve the StateCommand
		auto stateCommand = scene->GetNextState();

		// Check if a command was issued
		if (stateCommand)
		{
			// Check contents of the command
			// -- Check if the state is suiciding
			if (stateCommand->GetKilled())
			{
				Cleanup();

				// We must pop first, otherwise we will push and pop the new one instead
				theGSM->PopState();
			}

			// -- Check the Next State
			if (CGameState* newState = stateCommand->GetState())
			{
				// A state was provided so we add it onto the stack
				theGSM->PushState(newState);

				// Check if this next state is the play state
				CPlayState* playState = dynamic_cast<CPlayState*>(newState);

				if (playState)
				{
					// Retrieve the tag and init it
					string tag = stateCommand->GetTag();
					playState->Init(tag, theGSM->GetWindowWidth(), theGSM->GetWindowHeight());
				}
			}

			// Check if a tag receiver was provided, if so, pass it on to the tagReceiver for it to work with it
			if (tagReceiver)
			{
				*tagReceiver = stateCommand->GetTag();
			}
		}
	}
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