#include "playstate.h"

#include "../gamestate.h"
#include "../GameStateManager.h"
#include "PauseState.h"
#include "..\CustomScenes\GameScene.h"

CPlayState CPlayState::thePlayState;

CPlayState::~CPlayState()
{
}

void CPlayState::Init(const int width, const int height)
{
#if GSM_DEBUG_MODE
	cout << "CPlayState::Init\n" << endl;
#endif
	counter = 0;

	scene = new GameScene(width, height);
	scene->Init();
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
	if (key == 27)		// Escape key
	{
		theGSM->PushState(PauseState::Instance());
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

CPlayState* CPlayState::Instance()
{
	return &thePlayState;
}

CPlayState::CPlayState()
{
}