#include "MenuScene.h"

#include "../MeshBuilder.h"
#include "../LoadTGA.h"
#include <sstream>
#include "../Application.h"

using std::ostringstream;

MenuScene::MenuScene(const int window_width, const int window_height) : CSceneManager(window_width, window_height)
	, m_leftMouseState(UP_STATE)
	, m_rawLeftClick(false)
	, m_menuTitle(nullptr)
	, m_defaultBG(nullptr)
	, m_bg(nullptr)
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

	// Set up the default background
	m_defaultBG = MeshBuilder::GenerateQuad("bg", Color(0.1f, 0.1f, 0.1f), 1.0f);
	m_defaultBG->textureID = LoadTGA("Image//BG.tga");

	// Set the background to the default
	m_bg = m_defaultBG;
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

	if (m_defaultBG)
	{
		delete m_defaultBG;
		m_defaultBG = nullptr;
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

void MenuScene::createTitle(Mesh* titleMesh)
{
	m_menuTitle = titleMesh;
}

void MenuScene::createBackground(Mesh* bgMesh)
{
	m_bg = bgMesh;

	// Since a custom one was set, let's delete the one we have
	if (m_defaultBG)
	{
		delete m_defaultBG;
		m_defaultBG = nullptr;
	}
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

	// Render the title
	if (m_menuTitle)
	{
		Render2DMesh(m_menuTitle, false, 600, 150, m_window_width * 0.5f, m_window_height * 0.8f);
	}

	// Render the BG
	if (m_bg)
	{
		Render2DMesh(m_bg, false, m_window_width, m_window_height, m_window_width * 0.5f, m_window_height * 0.5f);
	}
}