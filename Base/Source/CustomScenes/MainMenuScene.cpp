#include "MainMenuScene.h"

#include "../MeshBuilder.h"
#include "../LoadTGA.h"
#include <sstream>
#include "../Application.h"
#include "../CustomStates/playState.h"
#include "../CustomStates/OptionState.h"

using std::ostringstream;

MainMenuScene::MainMenuScene(const int window_width, const int window_height) : MenuScene(window_width, window_height)
{
}

MainMenuScene::~MainMenuScene()
{
}

void MainMenuScene::Init()
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
	meshList[GEO_BT_START] = MeshBuilder::GenerateQuad("btn_start", Color(), 1.0f);
	meshList[GEO_BT_START]->textureID = LoadTGA("Image//btn_start.tga");
	meshList[GEO_BT_OPTIONS] = MeshBuilder::GenerateQuad("btn_options", Color(), 1.0f);
	meshList[GEO_BT_OPTIONS]->textureID = LoadTGA("Image//btn_options.tga");
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	// Initialize the buttons
	const Vector3 NORMAL_BUTTON_SIZE(250, 50);
	const Vector3 SCALE_BUTTON_SIZE(50, 50);

	createButtonList(BT_TOTAL);
	m_button[BT_START].Init(meshList[GEO_BT_START], Vector3(m_window_width * 0.5, m_window_height * 0.6), NORMAL_BUTTON_SIZE);
	m_button[BT_OPTIONS].Init(meshList[GEO_BT_OPTIONS], Vector3(m_window_width * 0.5, m_window_height * 0.4), NORMAL_BUTTON_SIZE);
}

void MainMenuScene::Update(double dt)
{
	MenuScene::Update(dt);

	if (m_button[BT_START].GetState() == UIButton::DOWN_STATE)
	{
		changeState(CPlayState::Instance());
	}
	else if (m_button[BT_OPTIONS].GetState() == UIButton::DOWN_STATE)
	{
		changeState(OptionState::Instance());
	}
}

void MainMenuScene::Render()
{
	MenuScene::Render();

	RenderMesh(meshList[GEO_AXES], false);
}

void MainMenuScene::Exit()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}

	MenuScene::Exit();
}

void MainMenuScene::UpdateWeaponStatus(const unsigned char key)
{
	MenuScene::UpdateWeaponStatus(key);
}