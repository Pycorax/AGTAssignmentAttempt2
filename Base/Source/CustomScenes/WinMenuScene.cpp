#include "WinMenuScene.h"

#include "../MeshBuilder.h"
#include "../LoadTGA.h"
#include <sstream>
#include "../Application.h"
#include "../CustomStates/LoseState.h"
#include "../Highscore/HighscoreSystem.h"

using std::ostringstream;

WinMenuScene::WinMenuScene(const int window_width, const int window_height, int score) : MenuScene(window_width, window_height)
{
}

WinMenuScene::~WinMenuScene()
{
}

void WinMenuScene::Init()
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
	meshList[GEO_TITLE]->textureID = LoadTGA("Image//title_win.tga");
	meshList[GEO_BT_RETRY] = MeshBuilder::GenerateQuad("btn_retry", Color(), 1.0f);
	meshList[GEO_BT_RETRY]->textureID = LoadTGA("Image//btn_retry.tga");
	meshList[GEO_BT_QUIT] = MeshBuilder::GenerateQuad("btn_quit", Color(), 1.0f);
	meshList[GEO_BT_QUIT]->textureID = LoadTGA("Image//btn_quit.tga");
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
	m_button[BT_QUIT].Init(meshList[GEO_BT_QUIT], Vector3(m_window_width * 0.5, m_window_height * 0.3), NORMAL_BUTTON_SIZE);
}

void WinMenuScene::Update(double dt)
{
	MenuScene::Update(dt);

	if (m_button[BT_QUIT].GetState() == UIButton::DOWN_STATE)
	{
		changeState(nullptr, true, LoseState::TAG_RETURN_TO_MENU);
	}
}

void WinMenuScene::Render()
{
	// Render the score
	ostringstream oss;
	oss << "Score: " << m_score;
	MenuScene::Render(); 
	
	RenderTextOnScreen(meshList[GEO_TEXT], oss.str(), Color(0.0f, 0.0f, 0.0f), 30, (m_window_width - oss.str().length() * 15)  * 0.5f, m_window_height * 0.6);
}

void WinMenuScene::Exit()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}

	MenuScene::Exit();
}

void WinMenuScene::UpdateWeaponStatus(const unsigned char key)
{
	MenuScene::UpdateWeaponStatus(key);
}