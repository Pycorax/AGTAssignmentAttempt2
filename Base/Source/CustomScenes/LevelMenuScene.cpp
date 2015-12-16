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
	meshList[GEO_BT_BACK] = MeshBuilder::GenerateQuad("btn_back", Color(), 1.0f);
	meshList[GEO_BT_BACK]->textureID = LoadTGA("Image//btn_back.tga");
	meshList[GEO_BT_LEVEL] = MeshBuilder::GenerateQuad("btn_level", Color(), 1.0f);
	meshList[GEO_BT_LEVEL]->textureID = LoadTGA("Image//btn_level.tga");
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	// Initialize the buttons
	const Vector3 NORMAL_BUTTON_SIZE(250, 50);
	const Vector3 SCALE_BUTTON_SIZE(50, 50);

	createButtonList(BT_TOTAL);
	m_button[BT_BACK].Init(meshList[GEO_BT_BACK], Vector3(m_window_width * 0.3, m_window_height * 0.1), NORMAL_BUTTON_SIZE);

	// Make every level button do the same thing
	for (int col = 0; col < BUTTON_COLS; ++col)
	{
		for (int row = 0; row < BUTTON_ROWS; ++row)
		{
			m_button[BT_LEVEL + (row * BUTTON_COLS) + col].Init(meshList[GEO_BT_LEVEL], Vector3(m_window_width * (0.3 + 0.4 * col), m_window_height * (0.7 - 0.15 * row)), NORMAL_BUTTON_SIZE);
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

	for (size_t i = 0; i < BUTTON_COLS * BUTTON_ROWS; ++i)
	{
		if (m_button[BT_LEVEL+  i].GetState() == UIButton::DOWN_STATE)
		{
			changeState(CPlayState::Instance(), true);
		}
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