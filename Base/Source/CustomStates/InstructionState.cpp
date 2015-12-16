#include "InstructionState.h"

#include "../GameStateManager.h"
#include "playstate.h"
#include "../gamestate.h"
#include "../CustomScenes/InstructionMenuScene.h"

InstructionState InstructionState::theMenuState;

void InstructionState::Init(const int width, const int height)
{
	scene = new InstructionMenuScene(width, height);
	scene->Init();
}

void InstructionState::Cleanup()
{
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void InstructionState::Pause()
{

}

void InstructionState::Resume()
{

}

void InstructionState::HandleEvents(CGameStateManager* theGSM)
{

}

void InstructionState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{

}

void InstructionState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right)
{
	if (button_Left == true)
		scene->UpdateWeaponStatus(scene->WA_FIRE);
}

void InstructionState::Update(CGameStateManager* theGSM) 
{
	scene->Update(0.16667);

	if (scene->HasEnded())
	{
		theGSM->ChangeState(CPlayState::Instance());
	}
}

void InstructionState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
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

void InstructionState::Draw(CGameStateManager* theGSM) 
{
	scene->Render();
}
