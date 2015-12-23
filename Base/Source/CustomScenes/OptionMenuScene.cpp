#include "OptionMenuScene.h"

#include "../MeshBuilder.h"
#include "../LoadTGA.h"
#include <sstream>
#include "../Application.h"
#include "../CustomStates/menustate.h"

using std::ostringstream;

OptionMenuScene::OptionMenuScene(const int window_width, const int window_height) : MenuScene(window_width, window_height)
{
}

OptionMenuScene::~OptionMenuScene()
{
}

void OptionMenuScene::Init()
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
	meshList[GEO_TITLE]->textureID = LoadTGA("Image//title_options.tga");
	meshList[GEO_BT_BACK] = MeshBuilder::GenerateQuad("btn_back", Color(), 1.0f);
	meshList[GEO_BT_BACK]->textureID = LoadTGA("Image//btn_back.tga");
	meshList[GEO_BT_MUSIC_ON] = MeshBuilder::GenerateQuad("btn_musicOn", Color(), 1.0f);
	meshList[GEO_BT_MUSIC_ON]->textureID = LoadTGA("Image//btn_musicOn.tga");
	meshList[GEO_BT_MUSIC_OFF] = MeshBuilder::GenerateQuad("btn_musicOff", Color(), 1.0f);
	meshList[GEO_BT_MUSIC_OFF]->textureID = LoadTGA("Image//btn_musicOff.tga");
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
	m_button[BT_BACK].Init(meshList[GEO_BT_BACK], Vector3(m_window_width * 0.5, m_window_height * 0.4), NORMAL_BUTTON_SIZE);
	m_button[BT_MUSIC_TOGGLE].Init(meshList[GEO_BT_MUSIC_ON], Vector3(m_window_width * 0.5, m_window_height * 0.6), NORMAL_BUTTON_SIZE);

	// Set the correct mesh according to music state
	if (Application::IsBGMPlaying())
	{
		m_button[BT_MUSIC_TOGGLE].SetMesh(meshList[GEO_BT_MUSIC_ON]);
	}
	else
	{
		m_button[BT_MUSIC_TOGGLE].SetMesh(meshList[GEO_BT_MUSIC_OFF]);
	}
}

void OptionMenuScene::Update(double dt)
{
	MenuScene::Update(dt);

	if (m_button[BT_BACK].GetState() == UIButton::DOWN_STATE)
	{
		endState();
	}
	else if (m_button[BT_MUSIC_TOGGLE].GetState() == UIButton::DOWN_STATE)
	{
		if (Application::IsBGMPlaying())
		{
			Application::StopSound();
			m_button[BT_MUSIC_TOGGLE].SetMesh(meshList[GEO_BT_MUSIC_OFF]);
		}
		else
		{
			Application::StartSound();
			m_button[BT_MUSIC_TOGGLE].SetMesh(meshList[GEO_BT_MUSIC_ON]);
		}
	}
}

void OptionMenuScene::Render()
{
	MenuScene::Render();

	RenderMesh(meshList[GEO_AXES], false);
}

void OptionMenuScene::Exit()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}

	MenuScene::Exit();
}

void OptionMenuScene::UpdateWeaponStatus(const unsigned char key)
{
	MenuScene::UpdateWeaponStatus(key);
}