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

void CGameState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	// Check if there is a scene
	if (scene)
	{
		// Update the scene
		scene->Update(m_dElapsedTime);

		// If a new state is specified, load it.
		// Store this so that if this current state is to be popped, we don't push this first and end up popping the new one instead of the old one
		CGameState* nextState = scene->GetNextState();

		// Check if the scene has ended
		if (scene->HasEnded())
		{
			Cleanup();
			theGSM->PopState();
		}

		// Set up the next state if there is one
		if (nextState)
		{
			theGSM->PushState(nextState);
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