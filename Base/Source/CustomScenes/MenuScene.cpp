#include "MenuScene.h"

#include "../MeshBuilder.h"
#include "../LoadTGA.h"
#include <sstream>
#include "../Application.h"

using std::ostringstream;

MenuScene::MenuScene(const int window_width, const int window_height) : CSceneManager(window_width, window_height)
	, m_leftMouseState(UP_STATE)
	, m_rawLeftClick(false)
	, m_button(nullptr)
	, m_buttonSize(0)
{
}

MenuScene::~MenuScene()
{
}

void MenuScene::Init()
{
	CSceneManager::Init();

	// Set Mouse Mode
	Application::SetCursorShown();

	// Set the bg col
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void MenuScene::Update(double dt)
{
	CSceneManager::Update(dt);

	mouseUpdate();
	updateUIButtons();
}

void MenuScene::Render()
{
	CSceneManager::Render();
	RenderGUI();
}

void MenuScene::Exit()
{
	if (m_button)
	{
		delete[] m_button;
		m_button = nullptr;
	}

	CSceneManager::Exit();
}

void MenuScene::UpdateWeaponStatus(const unsigned char key)
{
	if (key == WA_FIRE)
	{
		m_rawLeftClick = true;
	}
}

bool MenuScene::isMouseState(MOUSE_STATE_TYPE mouseState)
{
	return m_leftMouseState == mouseState;
}

void MenuScene::createButtonList(int sizeOfList)
{
	m_button = new UIButton[sizeOfList];
	m_buttonSize = sizeOfList;
}

void MenuScene::mouseUpdate()
{
	// Code to prevent holding the mouse key causing button presses to propagate through buttons in the same location on different menus
	switch (m_leftMouseState)
	{
		case UP_STATE:
			if (m_rawLeftClick)
			{
				m_leftMouseState = CLICK_DOWN_STATE;
			}
			break;
		case CLICK_DOWN_STATE:
			if (!m_rawLeftClick)
			{
				m_leftMouseState = CLICK_RELEASE_STATE;
			}
			else
			{
				m_leftMouseState = CLICK_HELD_STATE;
			}
			break;
		case CLICK_HELD_STATE:
			if (!m_rawLeftClick)
			{
				m_leftMouseState = CLICK_RELEASE_STATE;
			}
			break;
		case CLICK_RELEASE_STATE:
			if (!m_rawLeftClick)
			{
				m_leftMouseState = UP_STATE;
			}
			break;
		default:
			break;
	}

	m_rawLeftClick = false;
}

void MenuScene::updateUIButtons()
{
	for (size_t i = 0; i < m_buttonSize; ++i)
	{
		m_button[i].UpdateState(m_leftMouseState == CLICK_DOWN_STATE, m_window_height);
	}
}

void MenuScene::renderUIButton(UIButton btn)
{
	Vector3 btnPos = btn.GetPosition();
	Vector3 btnScale = btn.GetScale();

	Render2DMesh(btn.GetMesh(), false, btnScale.x, btnScale.y, btnPos.x, btnPos.y);
}

void MenuScene::OnResume()
{
	CSceneManager::OnResume();
	
	// Hide the Cursor
	Application::SetCursorShown();
}

/********************************************************************************
Render GUI
********************************************************************************/
void MenuScene::RenderGUI()
{
	// Render the buttons
	for (size_t bt = 0; bt < m_buttonSize; ++bt)
	{
		renderUIButton(m_button[bt]);
	}
}