#include "OptionState.h"

#include <iostream>

#include "../GameStateManager.h"
#include "playstate.h"
#include "../gamestate.h"
#include "../CustomScenes/OptionMenuScene.h"

OptionState OptionState::theMenuState;

void OptionState::Init(const int width, const int height)
{
	scene = new OptionMenuScene(width, height);
	scene->Init();
}

void OptionState::Cleanup()
{
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void OptionState::Pause()
{

}

void OptionState::Resume()
{

}

void OptionState::HandleEvents(CGameStateManager* theGSM)
{

}

void OptionState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{

}

void OptionState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right)
{
	if (button_Left == true)
		scene->UpdateWeaponStatus(scene->WA_FIRE);
}

void OptionState::Update(CGameStateManager* theGSM) 
{
	scene->Update(0.16667);

	if (scene->HasEnded())
	{
		theGSM->ChangeState(CPlayState::Instance());
	}
}

void OptionState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
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

void OptionState::Draw(CGameStateManager* theGSM) 
{
	scene->Render();
}
