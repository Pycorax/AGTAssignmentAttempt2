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

		// Chceck if the scene has ended
		if (scene->HasEnded())
		{
			// If a new state is specified, load it
			if (scene->GetNextState() != nullptr)
			{
				theGSM->ChangeState(scene->GetNextState());
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