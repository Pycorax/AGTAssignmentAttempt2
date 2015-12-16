#include "GameState.h"

CGameState::~CGameState()
{
}

void CGameState::Cleanup()
{
	// If thre is a scene
	if (scene)
	{
		// Delete the scene
		scene->Exit();
		delete scene;
		scene = nullptr;
	}
}

void CGameState::Pause()
{
	if (scene)
	{
		scene->OnPause();
	}
}

void CGameState::Resume()
{
	if (scene)
	{
		scene->OnResume();
	}
}

void CGameState::Update(CGameStateManager* theGSM)
{
	Update(theGSM, 0.16667);
}

void CGameState::Update(CGameStateManager* theGSM, const double m_dElapsedTime, string* tagReceiver)
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
			}

			// Check if a tag receiver was provided, if so, pass it on to the tagReceiver for it to work with it
			if (tagReceiver)
			{
				*tagReceiver = stateCommand->GetTag();
			}
		}
	}
}

void CGameState::Draw(CGameStateManager* theGSM)
{
	if (scene)
	{
		scene->Render();
	}
}

void CGameState::ChangeState(CGameStateManager* theGSM, CGameState* state)
{
	theGSM->ChangeState(state);
}

CGameState::CGameState()
	: scene(nullptr)
{
}