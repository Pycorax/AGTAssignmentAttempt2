#include "PauseMenuScene.h"

#include "../MeshBuilder.h"
#include "../LoadTGA.h"
#include <sstream>
#include "../Application.h"
#include "../CustomStates/OptionState.h"
#include "../CustomStates/PauseState.h"

using std::ostringstream;

PauseMenuScene::PauseMenuScene(const int window_width, const int window_height) : MenuScene(window_width, window_height)
{
}

PauseMenuScene::~PauseMenuScene()
{
}

void PauseMenuScene::Init()
{
	MenuScene::Init();

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
	meshList[GEO_BT_BACK] = MeshBuilder::GenerateQuad("btn_back", Color(), 1.0f);
	meshList[GEO_BT_BACK]->textureID = LoadTGA("Image//btn_back.tga");
	meshList[GEO_BT_OPTIONS] = MeshBuilder::GenerateQuad("btn_options", Color(), 1.0f);
	meshList[GEO_BT_OPTIONS]->textureID = LoadTGA("Image//btn_options.tga");
	meshList[GEO_BT_QUIT] = MeshBuilder::GenerateQuad("btn_quit", Color(), 1.0f);
	meshList[GEO_BT_QUIT]->textureID = LoadTGA("Image//btn_quit.tga");
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	// Initialize the buttons
	const Vector3 NORMAL_BUTTON_SIZE(250, 50);
	const Vector3 SCALE_BUTTON_SIZE(50, 50);

	createButtonList(BT_TOTAL);
	m_button[BT_BACK].Init(meshList[GEO_BT_BACK], Vector3(m_window_width * 0.5, m_window_height * 0.6), NORMAL_BUTTON_SIZE);
	m_button[BT_OPTIONS].Init(meshList[GEO_BT_OPTIONS], Vector3(m_window_width * 0.5, m_window_height * 0.5), NORMAL_BUTTON_SIZE);
	m_button[BT_QUIT].Init(meshList[GEO_BT_QUIT], Vector3(m_window_width * 0.5, m_window_height * 0.4), NORMAL_BUTTON_SIZE);
}

void PauseMenuScene::Update(double dt)
{
	MenuScene::Update(dt);

	if (m_button[BT_BACK].GetState() == UIButton::DOWN_STATE)
	{
		endState();
	}
	else if (m_button[BT_OPTIONS].GetState() == UIButton::DOWN_STATE)
	{
		changeState(OptionState::Instance());
	}
	else if (m_button[BT_QUIT].GetState() == UIButton::DOWN_STATE)
	{
		changeState(nullptr, true, PauseState::TAG_RETURN_TO_MENU);
	}
}

void PauseMenuScene::Render()
{
	MenuScene::Render();

	RenderMesh(meshList[GEO_AXES], false);
}

void PauseMenuScene::Exit()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}

	MenuScene::Exit();
}

void PauseMenuScene::UpdateWeaponStatus(const unsigned char key)
{
	MenuScene::UpdateWeaponStatus(key);
}