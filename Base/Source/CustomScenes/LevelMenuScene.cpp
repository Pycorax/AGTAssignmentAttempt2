#include "LevelMenuScene.h"

#include "../MeshBuilder.h"
#include "../LoadTGA.h"
#include <sstream>
#include "../Application.h"
#include "../CustomStates/menustate.h"
#include "../CustomStates/playstate.h"

using std::ostringstream;

LevelMenuScene::LevelMenuScene(const int window_width, const int window_height) : MenuScene(window_width, window_height)
{
}

LevelMenuScene::~LevelMenuScene()
{
}

void LevelMenuScene::Init()
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
	meshList[GEO_TITLE] = MeshBuilder::GenerateQuad("title", Color(), 1.0f);
	meshList[GEO_TITLE]->textureID = LoadTGA("Image//title_levelSelect.tga");
	meshList[GEO_BT_BACK] = MeshBuilder::GenerateQuad("btn_back", Color(), 1.0f);
	meshList[GEO_BT_BACK]->textureID = LoadTGA("Image//btn_back.tga");
	meshList[GEO_BT_LEVEL_DEMO] = MeshBuilder::GenerateQuad("btn_level_demo", Color(), 1.0f);
	meshList[GEO_BT_LEVEL_DEMO]->textureID = LoadTGA("Image//btn_level_demo.tga");
	meshList[GEO_BT_LEVEL_1] = MeshBuilder::GenerateQuad("btn_level_1", Color(), 1.0f);
	meshList[GEO_BT_LEVEL_1]->textureID = LoadTGA("Image//btn_level_1.tga");
	meshList[GEO_BT_LEVEL_2] = MeshBuilder::GenerateQuad("btn_level_2", Color(), 1.0f);
	meshList[GEO_BT_LEVEL_2]->textureID = LoadTGA("Image//btn_level_2.tga");
	meshList[GEO_BT_LEVEL_3] = MeshBuilder::GenerateQuad("btn_level_3", Color(), 1.0f);
	meshList[GEO_BT_LEVEL_3]->textureID = LoadTGA("Image//btn_level_3.tga");
	meshList[GEO_BT_LEVEL_4] = MeshBuilder::GenerateQuad("btn_level_4", Color(), 1.0f);
	meshList[GEO_BT_LEVEL_4]->textureID = LoadTGA("Image//btn_level_4.tga");
	meshList[GEO_BT_LEVEL_5] = MeshBuilder::GenerateQuad("btn_level_5", Color(), 1.0f);
	meshList[GEO_BT_LEVEL_5]->textureID = LoadTGA("Image//btn_level_5.tga");
	meshList[GEO_BT_LEVEL_6] = MeshBuilder::GenerateQuad("btn_level_6", Color(), 1.0f);
	meshList[GEO_BT_LEVEL_6]->textureID = LoadTGA("Image//btn_level_6.tga");
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	// Set the title
	createTitle(meshList[GEO_TITLE]);

	// Initialize the buttons
	const Vector3 NORMAL_BUTTON_SIZE(250, 50);
	const Vector3 SCALE_BUTTON_SIZE(50, 50);

	createButtonList(BT_TOTAL);
	m_button[BT_BACK].Init(meshList[GEO_BT_BACK], Vector3(m_window_width * 0.3, m_window_height * 0.1), NORMAL_BUTTON_SIZE);
	m_button[BT_LEVEL_DEMO].Init(meshList[GEO_BT_LEVEL_DEMO], Vector3(m_window_width * 0.3, m_window_height * 0.6), NORMAL_BUTTON_SIZE);

	for (int col = 0; col < BUTTON_COLS; ++col)
	{
		for (int row = 0; row < BUTTON_ROWS; ++row)
		{
			if ((BT_LEVEL_SURVIVAL1 + (row * BUTTON_COLS) + col) > BT_LEVEL_SURVIVAL6)
			{
				break;
			}

			m_button[BT_LEVEL_SURVIVAL1 + (row * BUTTON_COLS) + col].Init(meshList[GEO_BT_LEVEL_1 + (row * BUTTON_COLS) + col], Vector3(m_window_width * (0.3 + 0.4 * col), m_window_height * (0.5 - 0.1 * row)), NORMAL_BUTTON_SIZE);
		}
	}
}

void LevelMenuScene::Update(double dt)
{
	MenuScene::Update(dt);

	if (m_button[BT_BACK].GetState() == UIButton::DOWN_STATE)
	{
		endState();
	}

	if (m_button[BT_LEVEL_DEMO].GetState() == UIButton::DOWN_STATE)
	{
		changeState(CPlayState::Instance(), true, "d");
	}

	if (m_button[BT_LEVEL_SURVIVAL1].GetState() == UIButton::DOWN_STATE)
	{
		changeState(CPlayState::Instance(), true, "s1111 Level 1");
	}

	if (m_button[BT_LEVEL_SURVIVAL2].GetState() == UIButton::DOWN_STATE)
	{
		changeState(CPlayState::Instance(), true, "s2112 Level 2");
	}

	if (m_button[BT_LEVEL_SURVIVAL3].GetState() == UIButton::DOWN_STATE)
	{
		changeState(CPlayState::Instance(), true, "s3212 Level 3");
	}

	if (m_button[BT_LEVEL_SURVIVAL4].GetState() == UIButton::DOWN_STATE)
	{
		changeState(CPlayState::Instance(), true, "s3323 Level 4");
	}

	if (m_button[BT_LEVEL_SURVIVAL5].GetState() == UIButton::DOWN_STATE)
	{
		changeState(CPlayState::Instance(), true, "s4231 Level 5");
	}

	if (m_button[BT_LEVEL_SURVIVAL6].GetState() == UIButton::DOWN_STATE)
	{
		changeState(CPlayState::Instance(), true, "s5555 Level 6");
	}
}

void LevelMenuScene::Render()
{
	MenuScene::Render();

	RenderMesh(meshList[GEO_AXES], false);
}

void LevelMenuScene::Exit()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}

	MenuScene::Exit();
}

void LevelMenuScene::UpdateWeaponStatus(const unsigned char key)
{
	MenuScene::UpdateWeaponStatus(key);
}