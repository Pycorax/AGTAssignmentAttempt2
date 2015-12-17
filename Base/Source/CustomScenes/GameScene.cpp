#include "GameScene.h"

#include "../MeshBuilder.h"
#include "../LoadTGA.h"
#include "../LoadOBJ.h"
#include <sstream>
#include "../Application.h"

using std::ostringstream;

GameScene::GameScene() : CSceneManager()
	, mover(nullptr)
{
}

GameScene::GameScene(const int window_width, const int window_height) : CSceneManager(window_width, window_height)
	, mover(nullptr)
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	CSceneManager::Init();

	// Hide the Cursor
	Application::SetCursorHidden();

	meshInit();

	// Initialise and load a model into it
	m_cAvatar = new CPlayInfo3PV();
	m_cAvatar->SetModel(meshList[GEO_PLAYER]);

	// Create a scenegraph
	m_cSceneGraph = new CSceneNode();

	CModel* newModel = new CModel(meshList[GEO_YELLOW_CUBE]);
	cout << m_cSceneGraph->SetNode(new CTransform(), newModel) << endl;

	newModel = new CModel(meshList[GEO_YELLOW_CUBE]);
	cout << m_cSceneGraph->AddChild(new CTransform(125, 10, 0), newModel) << endl;

	newModel = new CModel(meshList[GEO_YELLOW_CUBE]);
	cout << m_cSceneGraph->AddChild(new CTransform(125, 10, 125), newModel) << endl;

#pragma region Auto Switching Grid Showcase

	CTransform* tf = new CTransform(100, 10, 10);
	tf->SetScale(10, 10, 10);

	int store = 0;

	newModel = new CModel(meshList[GEO_YELLOW_CUBE]);
	store = m_cSceneGraph->AddChild(tf, newModel);
	cout << store << endl;
	mover = m_cSceneGraph->GetNode(store);
	
#pragma endregion

	bomberInit();

	// Create a spatial partition
	m_cSpatialPartition = new CSpatialPartition();
	m_cSpatialPartition->Init(50, 50, 5, 5);
	for (int i = 0; i<m_cSpatialPartition->GetxNumOfGrid(); i++)
	{
		for (int j = 0; j<m_cSpatialPartition->GetyNumOfGrid(); j++)
		{
			m_cSpatialPartition->SetGridMesh(i, j, MeshBuilder::GenerateQuad("GridMesh", Color(1.0f / i, 1.0f / j, 1.0f / (i*j)), 50.0f));
		}
	}

	//m_cSpatialPartition->PrintSelf();

	// Add the pointers to the scene graph to the spatial partition
	m_cSpatialPartition->AddObject(m_cSceneGraph);

	// Create the projectile manager
	m_cProjectileManager = new CProjectileManager();

	rotateAngle = 0;

	bLightEnabled = true;
}

void GameScene::Update(double dt)
{
	CSceneManager::Update(dt);

	m_cAvatar->Update(dt);
	camera.UpdatePosition(m_cAvatar->GetPosition(), m_cAvatar->GetDirection());

	// Update the spatial partition
	//m_cSpatialPartition->Update(camera.position, (camera.target - camera.position).Normalize());

	// Update the Projectile Manager
	m_cProjectileManager->Update(dt);

	// Check for collisions fpr Projectiles
	// Render the projectiles
	Vector3 ProjectilePosition;
	Vector3 ProjectilePosition_End;
	for (int i = 0; i<m_cProjectileManager->GetMaxNumberOfProjectiles(); i++)
	{
		if (m_cProjectileManager->IsActive(i))
		{
			ProjectilePosition = m_cProjectileManager->theListOfProjectiles[i]->GetPosition();

			if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PT_DISCRETE)
			{
				// Destroy the projectile after collision
				if (CSceneNode* node = m_cSpatialPartition->CheckForCollision(ProjectilePosition))
				{
					// Remove the projectile
					m_cProjectileManager->RemoveProjectile(i);
					// React accordingly to the collided item
					node->Deactivate();
				}
			}
			else if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PT_RAY)
			{
				ProjectilePosition_End = ProjectilePosition + m_cProjectileManager->theListOfProjectiles[i]->GetDirection() * m_cProjectileManager->theListOfProjectiles[i]->GetLength();

				// Destroy the ray projectile after collision
				if (m_cSpatialPartition->CheckForCollision(ProjectilePosition, ProjectilePosition_End) == true)
					m_cProjectileManager->RemoveProjectile(i);
			}
		}
	}

	// Update mobile objects
	mover->ApplyTranslate(0.0f, 0.0f, 5.0f * dt);
	static float moved = 0.0f;
	moved += 5.0f * dt;
	if (mover->GetTranslate().z > 250)
	{
		mover->ApplyTranslate(0.0f, 0.0f, -moved);
		moved = 0.0f;
	}
}

void GameScene::Render()
{
	CSceneManager::Render();

	RenderGround();
	RenderSkybox();
	RenderFixedObjects();
	RenderMobileObjects();

	RenderGUI();
}

void GameScene::Exit()
{
	// Clear Meshes
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}

	// Clear Bombers
	while (!m_bomberList.empty())
	{
		if (m_bomberList.back())
		{
			delete m_bomberList.back();
		}

		m_bomberList.pop_back();
	}

	CSceneManager::Exit();
}

void GameScene::UpdateWeaponStatus(const unsigned char key)
{
	if (key == WA_FIRE)
	{
		// Add a bullet object which starts at the camera position and moves in the camera's direction
		m_cProjectileManager->AddProjectile(camera.position, (camera.target - camera.position).Normalize(), 300.0f);
	}
	else if (key == WA_FIRE_SECONDARY)
	{
		m_cProjectileManager->AddRayProjectile(camera.position, (camera.target - camera.position).Normalize(), 300.0f);
	}
}

void GameScene::meshInit()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	meshList[GEO_YELLOW_CUBE] = MeshBuilder::GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
	meshList[GEO_RAY] = MeshBuilder::GenerateRay("Ray", 10.0f);
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair", 1.0f, 1.0f, 1.0f, 0.1f);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 0, 0), 18, 36, 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 0.1f);
	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	meshList[GEO_CONE]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	meshList[GEO_CONE]->material.kSpecular.Set(0.f, 0.f, 0.f);

	// Player
	meshList[GEO_PLAYER] = MeshBuilder::GenerateCube("player", Color(1.0f, 0.6f, 0.0f), 1.0f);

	// Human
	meshList[GEO_HUMAN_HAT] = MeshBuilder::GenerateCone("humanHat", Color(0.0f, 0.0f, 0.0f), 36, 1.f, 1.f);
	meshList[GEO_HUMAN_HEAD] = MeshBuilder::GenerateSphere("humanHead", Color(0.968f, 0.937f, 0.619f), 12, 12, 1.0f);
	meshList[GEO_HUMAN_BODY] = MeshBuilder::GenerateCone("humanBody", Color(0.282f, 0.568f, 0.803f), 36, 1.f, 1.f);

	// Sky Box
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("LEFT", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("RIGHT", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("TOP", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("BOTTOM", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("FRONT", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("BACK", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");

	// Load the ground mesh and texture
	meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	meshList[GEO_GROUND]->textureID = LoadTGA("Image//floor.tga");
}

void GameScene::bomberInit()
{
	// Adding Bombers to the Scene
	Bomber* bomber = new Bomber;
	bomber->Init(Vector3(52, 0, 52), meshList[GEO_HUMAN_HAT], meshList[GEO_HUMAN_HEAD], meshList[GEO_HUMAN_BODY]);
	m_cSceneGraph->AddChild(bomber->GetSceneGraph());
}


void GameScene::OnResume()
{
	CSceneManager::OnResume();

	// Hide the Cursor
	Application::SetCursorHidden();
}

/********************************************************************************
Render mobile objects
********************************************************************************/
void GameScene::RenderGUI()
{
	// Render the crosshair
	RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 10.0f);

	//On screen text
	std::ostringstream ss;
	ss.precision(5);

	ss << "Projectiles: " << m_cProjectileManager->NumOfActiveProjectile;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 6);

	ss.str("");
	ss << "Position: " << m_cAvatar->GetPosition();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 36);

	//RenderTextOnScreen(meshList[GEO_TEXT], "Hello Screen", Color(0, 1, 0), 3, 0, 0);
}

/********************************************************************************
Render mobile objects
********************************************************************************/
void GameScene::RenderMobileObjects()
{
	// Render LightBall
	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	// Draw the scene graph
	m_cSceneGraph->Draw(this);

	// Render the projectiles
	Vector3 ProjectilePosition;
	for (int i = 0; i<m_cProjectileManager->GetMaxNumberOfProjectiles(); i++)
	{
		if (m_cProjectileManager->IsActive(i))
		{
			ProjectilePosition = m_cProjectileManager->theListOfProjectiles[i]->GetPosition();
			modelStack.PushMatrix();
			modelStack.Translate(ProjectilePosition.x, ProjectilePosition.y, ProjectilePosition.z);
			if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PT_DISCRETE)
			{
				RenderMesh(meshList[GEO_SPHERE], false);
			}
			else if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PT_RAY)
			{
				glLineWidth(5.0f);
				modelStack.Rotate(m_cProjectileManager->theListOfProjectiles[i]->GetRotationZ(), 0, 0, 1);
				modelStack.Rotate(m_cProjectileManager->theListOfProjectiles[i]->GetRotationY(), 0, 1, 0);
				RenderMesh(meshList[GEO_RAY], false);
				glLineWidth(1.0f);
			}
			modelStack.PopMatrix();
		}
	}

	// Render the Avatar
	modelStack.PushMatrix();
	modelStack.Translate(m_cAvatar->GetPos_x(), m_cAvatar->GetPos_y() + 4.0f, m_cAvatar->GetPos_z());
	modelStack.Rotate(m_cAvatar->GetYRotation(), 0, 1, 0);
	modelStack.Scale(5.0f, 8.0f, 5.0f);
	RenderMesh(m_cAvatar->theAvatarMesh, false);
	modelStack.PopMatrix();
}

/********************************************************************************
Render the lights in this scene
********************************************************************************/
void GameScene::RenderFixedObjects()
{
	RenderMesh(meshList[GEO_AXES], false);
}


/********************************************************************************
Render the ground in this scene
********************************************************************************/
void GameScene::RenderGround()
{
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	{
		// Render Spatial Partitioning Grids
		modelStack.PushMatrix();
		modelStack.Translate(m_cSpatialPartition->GetGridSizeX() * 0.5f, -m_cSpatialPartition->GetGridSizeY() * 0.5f, 1);

		 cout << "Rendering..." << endl;
		for (int i = 0; i<m_cSpatialPartition->GetxNumOfGrid(); i++)
		{
			for (int j = 0; j<m_cSpatialPartition->GetyNumOfGrid(); j++)
			{
				cout << m_cSpatialPartition->GetGridItemSize(i, j) << endl;

				// Only render it if there is something in it
				if (m_cSpatialPartition->GetGridActiveItemCount(i, j) <= 0)
				{
					continue;
				}

				modelStack.PushMatrix();
				modelStack.Translate(m_cSpatialPartition->xGridSize*i, -m_cSpatialPartition->yGridSize*j, 0.0f);
				Mesh* t = m_cSpatialPartition->GetGridMesh(i, j);
				RenderMesh(t, false);
				modelStack.PopMatrix();
			}
		}
		cout << "Rendering ENDS" << endl;
		modelStack.PopMatrix();
	}
	{
		modelStack.PushMatrix();
		modelStack.Translate(125, -125, 0);
		//modelStack.Rotate(-90, 0, 0, 1);
		modelStack.Scale(250.0f, 250.0f, 1.0f);
		RenderMesh(meshList[GEO_GROUND], false);
		modelStack.PopMatrix();
	}
	
	modelStack.PopMatrix();	
}

/********************************************************************************
Render the skybox in this scene
********************************************************************************/
void GameScene::RenderSkybox()
{
	//left
	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
}