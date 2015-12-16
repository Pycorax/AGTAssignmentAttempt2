#include <iostream>
using namespace std;

#include "../gamestate.h"
#include "../GameStateManager.h"
#include "playstate.h"
#include "menustate.h"
#include "..\CustomScenes\GameScene.h"

CPlayState CPlayState::thePlayState;

void CPlayState::Init(const int width, const int height)
{
#if GSM_DEBUG_MODE
	cout << "CPlayState::Init\n" << endl;
#endif
	counter = 0;

	scene = new GameScene(width, height);
	scene->Init();
}

void CPlayState::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CPlayState::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void CPlayState::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CPlayState::Pause\n" << endl;
#endif
}

void CPlayState::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CPlayState::Resume\n" << endl;
#endif
}

void CPlayState::HandleEvents(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	//int m_iUserChoice = -1;

	//do {
	//	cout << "CPlayState: Choose one <0> Go to Menu State : " ;
	//	cin >> m_iUserChoice;
	//	cin.get();

	//	switch (m_iUserChoice) {
	//		case 0:
	//			theGSM->ChangeState( CMenuState::Instance() );
	//			break;
	//		default:
	//			cout << "Invalid choice!\n";
	//			m_iUserChoice = -1;
	//			break;
	//	}
	//} while (m_iUserChoice == -1);
#endif
}

void CPlayState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
#if GSM_DEBUG_MODE
	//int m_iUserChoice = -1;

	//do {
	//	cout << "CPlayState: Choose one <0> Go to Menu State : " ;
	//	cin >> m_iUserChoice;
	//	cin.get();

	//	switch (m_iUserChoice) {
	//		case 0:
	//			theGSM->ChangeState( CMenuState::Instance() );
	//			break;
	//		default:
	//			cout << "Invalid choice!\n";
	//			m_iUserChoice = -1;
	//			break;
	//	}
	//} while (m_iUserChoice == -1);
#endif
	if (key == 32)
	{
		//theGSM->ChangeState( CMenuState::Instance() );
	}
	else
	{
		scene->UpdateAvatarStatus( key, status );
		scene->UpdateCameraStatus(key, status);
	}
}

void CPlayState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right)
{
#if GSM_DEBUG_MODE
	//int m_iUserChoice = -1;

	//do {
	//	cout << "CPlayState: Choose one <0> Go to Menu State : " ;
	//	cin >> m_iUserChoice;
	//	cin.get();

	//	switch (m_iUserChoice) {
	//		case 0:
	//			theGSM->ChangeState( CMenuState::Instance() );
	//			break;
	//		default:
	//			cout << "Invalid choice!\n";
	//			m_iUserChoice = -1;
	//			break;
	//	}
	//} while (m_iUserChoice == -1);
#endif
	if (button_Left == true)
		scene->UpdateWeaponStatus(scene->WA_FIRE);
	else if (button_Right == true)
		scene->UpdateWeaponStatus(scene->WA_FIRE_SECONDARY);
}


void CPlayState::Update(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CPlayState::Update\n" << endl;
#endif
	// Update the scene
	scene->Update(0.16667);
}

void CPlayState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	// Update the scene
	scene->Update(m_dElapsedTime);

	if (scene->HasEnded())
	{
		if (scene->GetNextState() != nullptr)
		{
			theGSM->ChangeState(scene->GetNextState());
	}
	}
}

void CPlayState::Draw(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CPlayState::Draw : " << counter << "\n" << endl;
#endif

	// Render the scene
	scene->Render();
}
