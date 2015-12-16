#include "HighScoreState.h"

#include "../GameStateManager.h"
#include "playstate.h"
#include "../gamestate.h"
#include "../CustomScenes/HighScoreMenuScene.h"

HighScoreState HighScoreState::theMenuState;

void HighScoreState::Init(const int width, const int height)
{
	scene = new HighScoreMenuScene(width, height);
	scene->Init();
}

void HighScoreState::Cleanup()
{
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void HighScoreState::Pause()
{

}

void HighScoreState::Resume()
{

}

void HighScoreState::HandleEvents(CGameStateManager* theGSM)
{

}

void HighScoreState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{

}

void HighScoreState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right)
{
	if (button_Left == true)
		scene->UpdateWeaponStatus(scene->WA_FIRE);
}

void HighScoreState::Update(CGameStateManager* theGSM) 
{
	scene->Update(0.16667);

	if (scene->HasEnded())
	{
		theGSM->ChangeState(CPlayState::Instance());
	}
}

void HighScoreState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	scene->Update(m_dElapsedTime);

	if (scene->HasEnded())
	{
		if (scene->GetNextState() != nullptr)
		{
			theGSM->ChangeState(scene->GetNextState());
		}
	}
}

void HighScoreState::Draw(CGameStateManager* theGSM) 
{
	scene->Render();
}
