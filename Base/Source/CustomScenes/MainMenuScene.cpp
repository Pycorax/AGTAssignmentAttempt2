#include "MainMenuScene.h"

#include "../MeshBuilder.h"
#include "../LoadTGA.h"
#include <sstream>
#include "../Application.h"
#include "../CustomStates/LevelSelectState.h"
#include "../CustomStates/OptionState.h"
#include "../CustomStates/InstructionState.h"
#include "../CustomStates/HighScoreState.h"

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
	meshList[GEO_BG] = MeshBuilder::GenerateQuad("bg", Color(0.1f, 0.1f, 0.1f), 1.0f);
	meshList[GEO_BT_START] = MeshBuilder::GenerateQuad("btn_start", Color(), 1.0f);
	meshList[GEO_BT_START]->textureID = LoadTGA("Image//btn_start.tga");
	meshList[GEO_BT_OPTIONS] = MeshBuilder::GenerateQuad("btn_options", Color(), 1.0f);
	meshList[GEO_BT_OPTIONS]->textureID = LoadTGA("Image//btn_options.tga");
	meshList[GEO_BT_INSTRUCTIONS] = MeshBuilder::GenerateQuad("btn_instructions", Color(), 1.0f);
	meshList[GEO_BT_INSTRUCTIONS]->textureID = LoadTGA("Image//btn_instructions.tga");
	meshList[GEO_BT_HIGH_SCORE] = MeshBuilder::GenerateQuad("btn_highScores", Color(), 1.0f);
	meshList[GEO_BT_HIGH_SCORE]->textureID = LoadTGA("Image//btn_highScores.tga");
	meshList[GEO_BT_QUIT] = MeshBuilder::GenerateQuad("btn_quit", Color(), 1.0f);
	meshList[GEO_BT_QUIT]->textureID = LoadTGA("Image//btn_quit.tga");
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	// Initialize the BG
	createBackground(meshList[GEO_BG]);

	// Initialize the buttons
	const Vector3 NORMAL_BUTTON_SIZE(250, 50);
	const Vector3 SCALE_BUTTON_SIZE(50, 50);

	createButtonList(BT_TOTAL);
	m_button[BT_START].Init(meshList[GEO_BT_START], Vector3(m_window_width * 0.5, m_window_height * 0.5), NORMAL_BUTTON_SIZE);
	m_button[BT_INSTRUCTIONS].Init(meshList[GEO_BT_INSTRUCTIONS], Vector3(m_window_width * 0.5, m_window_height * 0.4), NORMAL_BUTTON_SIZE);
	m_button[BT_HIGH_SCORE].Init(meshList[GEO_BT_HIGH_SCORE], Vector3(m_window_width * 0.5, m_window_height * 0.3), NORMAL_BUTTON_SIZE);
	m_button[BT_OPTIONS].Init(meshList[GEO_BT_OPTIONS], Vector3(m_window_width * 0.5, m_window_height * 0.2), NORMAL_BUTTON_SIZE);
	m_button[BT_QUIT].Init(meshList[GEO_BT_QUIT], Vector3(m_window_width * 0.5, m_window_height * 0.1), NORMAL_BUTTON_SIZE);
}

void MainMenuScene::Update(double dt)
{
	MenuScene::Update(dt);

	if (m_button[BT_START].GetState() == UIButton::DOWN_STATE)
	{
		changeState(LevelSelectState::Instance());
	}
	else if (m_button[BT_OPTIONS].GetState() == UIButton::DOWN_STATE)
	{
		changeState(OptionState::Instance());
	}
	else if (m_button[BT_INSTRUCTIONS].GetState() == UIButton::DOWN_STATE)
	{
		changeState(InstructionState::Instance());
	}
	else if (m_button[BT_HIGH_SCORE].GetState() == UIButton::DOWN_STATE)
	{
		changeState(HighScoreState::Instance());
	}
	else if(m_button[BT_QUIT].GetState() == UIButton::DOWN_STATE)
	{
		endState();
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