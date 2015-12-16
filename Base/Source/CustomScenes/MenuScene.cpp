#include "MenuScene.h"

#include "../MeshBuilder.h"
#include "../LoadTGA.h"
#include <sstream>
#include "../Application.h"

using std::ostringstream;

MenuScene::MenuScene(const int window_width, const int window_height) : CSceneManager(window_width, window_height)
	, m_leftMouseState(UP_STATE)
	, m_rawLeftClick(false)
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

	// Init the mesh list
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	// Load the meshes
	meshList[GEO_BUTTON] = MeshBuilder::GenerateQuad("Button", Color(), 1.0f);
	meshList[GEO_BUTTON]->textureID = LoadTGA("Image//btn_start.tga");
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	// Initialize the buttons
	const Vector3 NORMAL_BUTTON_SIZE(250, 50);
	const Vector3 SCALE_BUTTON_SIZE(50, 50);
	m_button[BT_START].Init(meshList[GEO_BUTTON], Vector3(m_window_width * 0.5, m_window_height * 0.5), NORMAL_BUTTON_SIZE);
}

void MenuScene::Update(double dt)
{
	CSceneManager::Update(dt);

	mouseUpdate();

	updateUIButtons();

	if (m_button[BT_START].GetState() == UIButton::DOWN_STATE)
	{
		endScene();
	}
}

void MenuScene::Render()
{
	CSceneManager::Render();

	RenderMesh(meshList[GEO_AXES], false);

	RenderGUI();
}

void MenuScene::Exit()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
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
	}

	m_rawLeftClick = false;
}

void MenuScene::updateUIButtons()
{
	for (size_t i = 0; i < BT_TOTAL; ++i)
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

/********************************************************************************
Render GUI
********************************************************************************/
void MenuScene::RenderGUI()
{
	// Render the buttons
	for (size_t bt = 0; bt < BT_TOTAL; ++bt)
	{
		renderUIButton(m_button[bt]);
	}
}