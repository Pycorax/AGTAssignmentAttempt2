#include "SplashScene.h"

#include "../MeshBuilder.h"
#include "../LoadTGA.h"
#include <sstream>
#include "../Application.h"
#include "../CustomStates/menustate.h"

using std::ostringstream;

const float SplashScene::s_m_LIGHT_OFFSET = 50.0f;
const float SplashScene::s_m_LIGHT_SPEED = 20.0f;
const float SplashScene::s_m_END_TIME_PAUSE = 0.0f;

SplashScene::SplashScene() : CSceneManager()
	, m_timePause(0.0f)
{
}

SplashScene::SplashScene(const int window_width, const int window_height) : CSceneManager(window_width, window_height)
	, m_timePause(0.0f)
{
}

SplashScene::~SplashScene()
{
}

void SplashScene::Init()
{
	CSceneManager::Init();

	// Hide the Cursor
	Application::SetCursorHidden();

	// Set the bg col
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Init the mesh list
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	// Load the meshes
	meshList[GEO_LOGO] = MeshBuilder::GenerateOBJ("3DLogo", "OBJ//3dLogo.obj");
	meshList[GEO_LOGO]->textureID = LoadTGA("Image//3dLogo.tga");
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	// Set up the global light
	bLightEnabled = true;

	// Set up the lights[0]s
	lights[0].position.Set(-3.0f, 3.0f, -s_m_LIGHT_OFFSET);
	lights[0].power = 10.0f;

	// Init the camera
	camera.Init(Vector3(-5.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3::ZERO_VECTOR);
}

void SplashScene::Update(double dt)
{
	CSceneManager::Update(dt);

	if (lights[0].position.z < 0.0f)
	{
		lights[0].position.z += s_m_LIGHT_SPEED * 2 * dt;
	}
	else if (lights[0].position.z < s_m_LIGHT_SPEED)
	{
		lights[0].position.z += s_m_LIGHT_SPEED * dt;
		lights[0].power -= s_m_LIGHT_SPEED * dt;
	}
	else if (m_timePause < s_m_END_TIME_PAUSE)
	{
		m_timePause += dt;
	}
	else
	{
		// End the scene
		changeState(CMenuState::Instance(), true);
	}
}

void SplashScene::Render()
{
	CSceneManager::Render();

	RenderFixedObjects();
	RenderGUI();
}

void SplashScene::Exit()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}

	CSceneManager::Exit();
}


/********************************************************************************
Render mobile objects
********************************************************************************/
void SplashScene::RenderGUI()
{
	
}

/********************************************************************************
Render the lights[0]s in this scene
********************************************************************************/
void SplashScene::RenderFixedObjects()
{
#ifdef _DEBUG
	//RenderMesh(meshList[GEO_AXES], false);
#endif
	
	// Render the Logo
	modelStack.PushMatrix();
	modelStack.Rotate(180.0f, 0, 1, 0);
	modelStack.Scale(3.0f, 3.0f, 3.0f);
	RenderMesh(meshList[GEO_LOGO], true);
	modelStack.PopMatrix();
}