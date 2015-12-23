#include "HighScoreMenuScene.h"

#include "../MeshBuilder.h"
#include "../LoadTGA.h"
#include <sstream>
#include "../Application.h"
#include "../CustomStates/menustate.h"

using std::ostringstream;

HighScoreMenuScene::HighScoreMenuScene(const int window_width, const int window_height) : MenuScene(window_width, window_height)
{
}

HighScoreMenuScene::~HighScoreMenuScene()
{
}

void HighScoreMenuScene::Init()
{
	MenuScene::Init();

	// Set Mouse Mode
	Application::SetCursorShown();

	// Set the bg col
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// Init the mesh list
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	// Load the meshes
	meshList[GEO_TITLE] = MeshBuilder::GenerateQuad("title", Color(), 1.0f);
	meshList[GEO_TITLE]->textureID = LoadTGA("Image//title_highScore.tga");
	meshList[GEO_BT_BACK] = MeshBuilder::GenerateQuad("btn_back", Color(), 1.0f);
	meshList[GEO_BT_BACK]->textureID = LoadTGA("Image//btn_back.tga");
	meshList[GEO_BT_RESET] = MeshBuilder::GenerateQuad("btn_reset", Color(), 1.0f);
	meshList[GEO_BT_RESET]->textureID = LoadTGA("Image//btn_reset.tga");
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
	m_button[BT_RESET].Init(meshList[GEO_BT_RESET], Vector3(m_window_width * 0.7, m_window_height * 0.1), NORMAL_BUTTON_SIZE);

	// Load list of records to display
	m_recordsToDisplay = HighScoreSystem::GetRecords();
}

void HighScoreMenuScene::Update(double dt)
{
	MenuScene::Update(dt);

	if (m_button[BT_BACK].GetState() == UIButton::DOWN_STATE)
	{
		endState();
	}
	if (m_button[BT_RESET].GetState() == UIButton::DOWN_STATE)
	{
		HighScoreSystem::Reset();
		m_recordsToDisplay = HighScoreSystem::GetRecords();
	}
}

void HighScoreMenuScene::Render()
{
	MenuScene::Render();

	// Render the high score records
	const Vector2 RECORD_START_POINT(m_window_width * 0.5f, m_window_height * 0.65f);
	int i = 0;
	for (auto record = m_recordsToDisplay.begin(); record != m_recordsToDisplay.end(); ++record)
	{
		// Create the string version of the record to print
		ostringstream oss;
		oss << record->levelName << " - " << record->score;

		RenderTextOnScreen(meshList[GEO_TEXT], oss.str(), Color(0.0f, 0.0f, 0.0f), 30, RECORD_START_POINT.x - (7.5f * oss.str().length()), RECORD_START_POINT.y - 37 * i);

		// Increment the i which controls the y
		++i;
	}
}

void HighScoreMenuScene::Exit()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}

	MenuScene::Exit();
}

void HighScoreMenuScene::UpdateWeaponStatus(const unsigned char key)
{
	MenuScene::UpdateWeaponStatus(key);
}