#include "GameScene.h"

// STL Includes
#include <sstream>
#include <fstream>

// API Includes
#include "LuaFunction.h"

// Other Includes
#include "../MeshBuilder.h"
#include "../LoadTGA.h"
#include "../LoadOBJ.h"
#include "../Application.h"
#include "../CustomStates/LoseState.h"
#include "../CustomStates/WinState.h"
#include "../Highscore/HighscoreSystem.h"

// Using Directives
using Lua::LuaFile;
using Lua::Type;
using std::ostringstream;
using std::ifstream;
using std::ofstream;

const float GameScene::INVULN_TIME = 2.0f;
const string GameScene::SAVE_FILE_PREFIX = "Saves//";
const string GameScene::SAVE_FILE_SUFFIX = "_save.lua";
const string GameScene::SAVE_NUM_ENEMIES_PROP_NAME = "GameNumEnemies";

GameScene::GameScene() : CSceneManager()
	, m_invulnTime(0.0f)
	, m_lives(MAX_LIVES)
	, m_numEnemiesAtStart(0)
	, m_lazerCollection(0)
	, m_score(0)
	, m_demoMode(false)
{
}

GameScene::GameScene(const int window_width, const int window_height) : CSceneManager(window_width, window_height)
	, m_invulnTime(0.0f)
	, m_lives(MAX_LIVES)
	, m_numEnemiesAtStart(0)
	, m_lazerCollection(0)
	, m_score(0)
	, m_demoMode(false)
{
}

GameScene::~GameScene()
{
}

void GameScene::Init(string levelString)
{
	CSceneManager::Init();

	// Hide the Cursor
	Application::SetCursorHidden();

	// Init the meshes
	meshInit();

	// Enable lighting
	bLightEnabled = true;

	// Load LUA
	// -- Load Game Properties from Lua
	LuaFile luaFile("Config//application_config.lua");

	/*
	 * Gameplay Init
	 */
	// Initialise and load a model into it
	m_cAvatar = new CPlayInfo3PV();
	m_cAvatar->SetModel(meshList[GEO_PLAYER_BODY]);
	
	// Create a scenegraph
	m_cSceneGraph = new CSceneNode();

	CModel* newModel = new CModel(nullptr);
	cout << m_cSceneGraph->SetNode(new CTransform(), newModel) << endl;

	// Create a spatial partition
	m_cSpatialPartition = new CSpatialPartition();
	m_cSpatialPartition->Init(luaFile.GetNumber("SPATIAL_PARTITION_GRID_WIDTH"), luaFile.GetNumber("SPATIAL_PARTITION_GRID_HEIGHT"), luaFile.GetNumber("SPATIAL_PARTITION_COL"), luaFile.GetNumber("SPATIAL_PARTITION_ROW"));
	for (int i = 0; i<m_cSpatialPartition->GetxNumOfGrid(); i++)
	{
		for (int j = 0; j<m_cSpatialPartition->GetyNumOfGrid(); j++)
		{
			m_cSpatialPartition->SetGridMesh(i, j, MeshBuilder::GenerateQuad("GridMesh", Color(1.0f / i, 1.0f / j, 1.0f / (i*j)), 1.0f));
		}
	}

	// Put the player in the middle
	m_cAvatar->SetPos_x(m_cSpatialPartition->GetGridSizeX() * m_cSpatialPartition->GetxNumOfGrid() * 0.5f);
	m_cAvatar->SetPos_z(m_cSpatialPartition->GetGridSizeY() * m_cSpatialPartition->GetyNumOfGrid() * 0.5f);

	
	// Get the level name
	if (levelString.length() > 0)
	{
		int nameStartPos = 0;
		int nameEndPos = levelString.length() - 1;
		// -- Start from the end and find a '/' if there is
		for (size_t i = levelString.length() - 1; i >= 0; --i)
		{
			if (levelString[i] == '/')
			{
				nameStartPos = i + 1;
				break;
			}
			else if (levelString[i] == '.')
			{
				nameEndPos = i;
			}
		}
		m_levelName = levelString.substr(nameStartPos, nameEndPos - nameStartPos);
	}
	else
	{
		m_levelName = "";
	}

	// Load Player Saved Status from File
	bool saveLoaded = false;
	try
	{
		LuaFile saveFile(SAVE_FILE_PREFIX + m_levelName + SAVE_FILE_SUFFIX);
		m_cAvatar->LoadStatus(&saveFile);

		// If a level string is provided, this means it's Survival mode
		if (m_levelName.length() > 0)
		{
			// Load Enemy Status
			int numEnemies = saveFile.GetNumber(SAVE_NUM_ENEMIES_PROP_NAME);
			for (size_t i = 0; i < numEnemies; ++i)
			{
				Bomber* bomber = new Bomber;
				bomber->LoadedInit(&saveFile, i, meshList[GEO_HUMAN_HAT], meshList[GEO_HUMAN_HEAD], meshList[GEO_HUMAN_BODY_LOW]);
				bomber->SetBodyLODModels(meshList[GEO_HUMAN_BODY_LOW], meshList[GEO_HUMAN_BODY_MED], meshList[GEO_HUMAN_BODY_HIGH]);
				bomber->SetFlock(&m_bomberList);
				m_cSceneGraph->AddChild(bomber);
				m_bomberList.push_back(bomber);
			}
		}

		// Mark that yes, we did load a save
		saveLoaded = true;
	}
	catch (runtime_error e)
	{
		// Nothing. No save file exists so we do nothing.
		cout << e.what() << endl;
	}
	catch (...)
	{

	}

	// If a level string is provided, this means it's Survival mode
	if (m_levelName.length() > 0)
	{
		// If not loaded by save, let's load it ourselves
		if (!saveLoaded)
		{
			// Load LUA
			// -- Load Game Properties from Lua
			LuaFile levelLuaScript(levelString);
			// -- Register C++ Functions
			levelLuaScript.RegisterFunction("spawnSurvivalBombers", bomberSurvivalInit);

			// Spawn the Enemies
			levelLuaScript.Call("levelInit", 0, Lua::LuaFuncList{ Lua::NewPtr(this) });
		}
	}
	else
	{
		bomberDemoInit();
	}

	// Add the pointers to the scene graph to the spatial partition
	m_cSpatialPartition->AddObject(m_cSceneGraph);

	// Create the projectile manager
	m_cProjectileManager = new CProjectileManager();

	// -- Set the projectile range
	m_cProjectileManager->Boundary_TopLeft.Set(m_cSpatialPartition->GetGridSizeX() * m_cSpatialPartition->GetxNumOfGrid(), 300.0f, m_cSpatialPartition->GetGridSizeY() * m_cSpatialPartition->GetyNumOfGrid());
	m_cProjectileManager->Boundary_BottomRight.Set(-m_cSpatialPartition->GetGridSizeX() * m_cSpatialPartition->GetxNumOfGrid(), -50.0f, -m_cSpatialPartition->GetGridSizeY() * m_cSpatialPartition->GetyNumOfGrid());

	// Give the player the max number of lives
	m_lives = MAX_LIVES;

	// Set up the player's weapons
	m_slowGun.Init(200, 1.5f, 800, 10);
	m_killGun.Init(1200, 0.0f, 100, 1);
}

void GameScene::Update(double dt)
{
	CSceneManager::Update(dt);

	// Update the Player
	m_cAvatar->Update(dt);
	m_cAvatar->ConstrainHero(0, m_cSpatialPartition->GetGridSizeX() * m_cSpatialPartition->GetxNumOfGrid(), 0, m_cSpatialPartition->GetGridSizeY() * m_cSpatialPartition->GetyNumOfGrid(), dt);

	// Update the Camera
	camera.UpdatePosition(m_cAvatar->GetPosition(), m_cAvatar->GetDirection(), m_cAvatar->GetUpDir(), m_cAvatar->IsSprinting(), dt);

	// Update the Spatial Partition for LODs
	m_cSpatialPartition->Update(camera.position, camera.target);

	// Update the player weapons
	m_slowGun.Update(dt);
	m_killGun.Update(dt);

	// Enemies
	bool gotHit = false;
	for (auto bomb = m_bomberList.begin(); bomb != m_bomberList.end(); ++bomb)
	{
		if ((*bomb)->Update(dt, m_cAvatar->GetPosition(), m_invulnTime > 0))
		{
			gotHit = true;
		}
	}

	// Update the Projectile Manager
	m_cProjectileManager->Update(dt);

	// Check for collisions for Projectiles
	// Update the projectiles
	Vector3 ProjectilePosition;
	Vector3 ProjectilePosition_PrevDTDist;
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
					// Check if the item is a bomber
					Bomber* bomber = dynamic_cast<Bomber*>(node);
					if (bomber && bomber->GetState() != Bomber::LS_MINE)
					{
						// Nudge the parent back
						bomber->GetParent()->Nudge(m_cProjectileManager->theListOfProjectiles[i]->GetDirection());
						m_lazerCollection++;
						// Update score
						m_score += SCORE_FOR_HIT;
					}
				}
			}
			else if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PT_RAY)
			{
				// Calculate the start pos of the previous distance missed by the previous DT
				ProjectilePosition_PrevDTDist = m_cProjectileManager->theListOfProjectiles[i]->GetDirection() * m_cProjectileManager->theListOfProjectiles[i]->GetSpeed() *dt;

				// Calculate the end pos of the ray
				ProjectilePosition_End = ProjectilePosition + m_cProjectileManager->theListOfProjectiles[i]->GetDirection() * m_cProjectileManager->theListOfProjectiles[i]->GetLength();				

				// Destroy the ray projectile after collision
				if (CSceneNode* node = m_cSpatialPartition->CheckForCollision(ProjectilePosition - ProjectilePosition_PrevDTDist, ProjectilePosition_End))
				{
					// Remove the projectile
					m_cProjectileManager->RemoveProjectile(i);
					// React accordingly to the collided item
					// Check if the item is a bomber
					Bomber* bomber = dynamic_cast<Bomber*>(node);
					if (bomber && bomber->GetState() != Bomber::LS_MINE)
					{
						bomber->Kill();

						// Update score
						m_score += SCORE_FOR_KILL;
					}
				}
			}
		}
	}

	// Update invulnerability timer
	if (m_invulnTime > 0.0f)
	{
		m_invulnTime -= dt;
	}

	// React to getting hit
	if (gotHit && m_invulnTime <= 0.0f)
	{
		killPlayer();
	}

	// Give the player a kill charge if he has enough points
	if (m_lazerCollection > LAZER_PRICE)
	{
		m_lazerCollection = 0;
		m_killGun.AddCharge();
	}

	// Check conditions for going to the end state
	checkEndState(dt);
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
	bool deleteFile = false;
	if (m_lives > 0 && m_bomberList.size() > 0 || m_demoMode)
	{
		// Save Data
		string saveLuaScript;
		// -- Save Player Status
		saveLuaScript += m_cAvatar->SaveStatus();

		if (!m_demoMode)
		{
			// -- Save Enemy Status
			int numActiveEnemies = 0;
			// ---- Save Enemy Status
			for (auto enemy : m_bomberList)
			{
				// Only save active enemies
				if (enemy->GetActive())
				{
					saveLuaScript += enemy->SaveStatus(numActiveEnemies);
					++numActiveEnemies;
				}
			}
			// ---- Save Number of Enemies
			saveLuaScript += LuaSerializable::BuildGenericPropString(SAVE_NUM_ENEMIES_PROP_NAME, to_string(numActiveEnemies));

			if (numActiveEnemies <= 0)
			{
				// If all the enemies are inactive, what's the point?
				deleteFile = true;
			}
		}

		// -- Check if anything has changed that would make it pointless to save
		if (!deleteFile)
		{
			// -- Generate a Save File
			ofstream saveFile(SAVE_FILE_PREFIX + m_levelName + SAVE_FILE_SUFFIX);
			if (saveFile.is_open())
			{
				// Write data to the savefile
				saveFile << saveLuaScript;

				// Close
				saveFile.close();
			}
			else
			{
				throw runtime_error("Unable to generate save file!");
			}
		}
	}
	else
	{
		deleteFile = true;
	}

	// We died or finished the level, so we delete this file
	if (deleteFile)
	{
		// We check if this exist first
		ifstream saveFile(SAVE_FILE_PREFIX + m_levelName + SAVE_FILE_SUFFIX);
		if (saveFile.good())
		{
			// Close this file
			saveFile.close();

			// Delete the save file
			remove((SAVE_FILE_PREFIX + m_levelName + SAVE_FILE_SUFFIX).c_str());
		}
	}

	// Clear Meshes
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
		{
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}

	// Clear Bombers
	while (!m_bomberList.empty())
	{
		// Don't delete as they will be deleted by the scenegraph
		m_bomberList.pop_back();
	}

	CSceneManager::Exit();
}

void GameScene::UpdateWeaponStatus(const unsigned char key)
{
	if (key == WA_FIRE && m_slowGun.Shoot() && m_cAvatar->IsSprinting() == false)
	{
		// Add a bullet object which starts at the camera position and moves in the camera's direction
		m_cProjectileManager->AddProjectile(camera.position, (camera.target - camera.position).Normalize(), m_slowGun.GetBulletSpeed());
	}
	else if (key == WA_FIRE_SECONDARY && m_killGun.Shoot() && m_cAvatar->IsSprinting() == false)
	{
		m_cProjectileManager->AddRayProjectile(camera.position, (camera.target - camera.position).Normalize(), m_killGun.GetBulletSpeed());
	}
}

void GameScene::meshInit()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	// Load Meshes from Lua
	LuaFile meshLoader("Config//mesh_loader.lua");
	meshLoader.RegisterFunction("loadMeshRay", loadMeshRay);
	meshLoader.RegisterFunction("loadMeshQuad", loadMeshQuad);
	meshLoader.RegisterFunction("loadMesh2DMesh", loadMesh2DMesh);
	meshLoader.RegisterFunction("loadMeshSphere", loadMeshSphere);
	meshLoader.RegisterFunction("loadMeshCone", loadMeshCone);
	meshLoader.RegisterFunction("loadMeshCrosshair", loadMeshCrosshair);
	meshLoader.Call("loadMeshes", 0, Lua::LuaFuncList{Lua::NewPtr(this)});

	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair", 1.0f, 1.0f, 1.0f, 0.1f);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 0, 0), 18, 36, 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 0.1f);

	// Player
	meshList[GEO_PLAYER_BODY] = MeshBuilder::GenerateCone("playerBody", Color(0.282f, 0.568f, 0.803f), 36, 1.f, 1.f);
	meshList[GEO_PLAYER_INVULN_HAT] = MeshBuilder::GenerateCone("invulnHat", Color(0.84f, 0.0f, 0.027f), 36, 1.f, 1.f);

	// Human
	meshList[GEO_HUMAN_HAT] = MeshBuilder::GenerateCone("humanHat", Color(0.0f, 0.0f, 0.0f), 36, 1.f, 1.f);
	
	meshList[GEO_HUMAN_HEAD] = MeshBuilder::GenerateSphere("humanHead", Color(0.968f, 0.937f, 0.619f), 12, 12, 1.0f);
	meshList[GEO_HUMAN_BODY_LOW] = MeshBuilder::GenerateCone("humanBodyLowRes", Color(0.83f, 0.11f, 0.14f), 4, 1.f, 1.f);
	meshList[GEO_HUMAN_BODY_MED] = MeshBuilder::GenerateCone("humanBodyMedRes", Color(0.83f, 0.11f, 0.14f), 8, 1.f, 1.f);
	meshList[GEO_HUMAN_BODY_HIGH] = MeshBuilder::GenerateCone("humanBodyHighRes", Color(0.83f, 0.11f, 0.14f), 36, 1.f, 1.f);

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

	// HUD
	meshList[GEO_LIFE] = MeshBuilder::Generate2DMesh("Life", Color(), 0, 0, 1, 1);
	meshList[GEO_LIFE]->textureID = LoadTGA("Image//HUD/life.tga");
	meshList[GEO_AMMO_BAR] = MeshBuilder::GenerateQuad("Ammo Bar", Color(0.65f, 0.87f, 0.97f));
	meshList[GEO_POWER_AMMO_BAR] = MeshBuilder::GenerateQuad("PAmmo Bar", Color(0.95f, 0.396f, 0.13f));
	meshList[GEO_KILL_BAR] = MeshBuilder::GenerateQuad("Score Bar", Color(0.22f, 0.71f, 0.29f));
	meshList[GEO_BAR_BG] = MeshBuilder::GenerateQuad("Bar BG", Color(0.54f, 0.54f, 0.54f));
}

void GameScene::loadMeshRay(string name, Color col, float length)
{
	m_meshResource[name] = MeshBuilder::GenerateRay(name, col, length);
}

void GameScene::loadMeshQuad(string name, Color col, float length)
{
	m_meshResource[name] = MeshBuilder::GenerateQuad(name, col, length);
}

void GameScene::loadMesh2DMesh(string name, Color col, int posX, int posY, int width, int height)
{
	m_meshResource[name] = MeshBuilder::Generate2DMesh(name, col, posX, posY, width, height);
}

void GameScene::loadMeshSphere(string name, Color col, int stack, int slice, float radius)
{
	m_meshResource[name] = MeshBuilder::GenerateSphere(name, col, stack, slice, radius);
}

void GameScene::loadMeshCone(string name, Color col, int slice, float radius, float height)
{
	m_meshResource[name] = MeshBuilder::GenerateCone(name, col, slice, radius, height);
}

void GameScene::loadMeshCrosshair(string name, Color col, float length)
{
	m_meshResource[name] = MeshBuilder::GenerateCrossHair(name, col.r, col.g, col.b, length);
}

void GameScene::bomberDemoInit()
{
	// Set the mode to demo
	m_demoMode = true;

	// Adding Bombers to the Scene
	Bomber* bomber;

	bomber = new Bomber;
	bomber->Init(Vector3(118, 0, 25), meshList[GEO_HUMAN_HAT], meshList[GEO_HUMAN_HEAD], meshList[GEO_HUMAN_BODY_LOW]);
	bomber->SetBodyLODModels(meshList[GEO_HUMAN_BODY_LOW], meshList[GEO_HUMAN_BODY_MED], meshList[GEO_HUMAN_BODY_HIGH]);
	m_cSceneGraph->AddChild(bomber);

	bomber = new Bomber;
	bomber->Init(Vector3(125, 0, 142), meshList[GEO_HUMAN_HAT], meshList[GEO_HUMAN_HEAD], meshList[GEO_HUMAN_BODY_LOW]);
	bomber->SetBodyLODModels(meshList[GEO_HUMAN_BODY_LOW], meshList[GEO_HUMAN_BODY_MED], meshList[GEO_HUMAN_BODY_HIGH]);
	m_cSceneGraph->AddChild(bomber);

	bomber = new Bomber;
	bomber->Init(Vector3(115, 0, 83), meshList[GEO_HUMAN_HAT], meshList[GEO_HUMAN_HEAD], meshList[GEO_HUMAN_BODY_LOW]);
	bomber->SetBodyLODModels(meshList[GEO_HUMAN_BODY_LOW], meshList[GEO_HUMAN_BODY_MED], meshList[GEO_HUMAN_BODY_HIGH]);
	m_cSceneGraph->AddChild(bomber);

	bomber = new Bomber;
	bomber->Init(Vector3(155, 0, 223), meshList[GEO_HUMAN_HAT], meshList[GEO_HUMAN_HEAD], meshList[GEO_HUMAN_BODY_LOW]);
	bomber->SetBodyLODModels(meshList[GEO_HUMAN_BODY_LOW], meshList[GEO_HUMAN_BODY_MED], meshList[GEO_HUMAN_BODY_HIGH]);
	m_cSceneGraph->AddChild(bomber);

	bomber = new Bomber;
	bomber->Init(Vector3(185, 0, 184), meshList[GEO_HUMAN_HAT], meshList[GEO_HUMAN_HEAD], meshList[GEO_HUMAN_BODY_LOW]);
	bomber->SetBodyLODModels(meshList[GEO_HUMAN_BODY_LOW], meshList[GEO_HUMAN_BODY_MED], meshList[GEO_HUMAN_BODY_HIGH]);
	m_cSceneGraph->AddChild(bomber);

	bomber = new Bomber;
	bomber->Init(Vector3(200, 0, 103), meshList[GEO_HUMAN_HAT], meshList[GEO_HUMAN_HEAD], meshList[GEO_HUMAN_BODY_LOW]);
	bomber->SetBodyLODModels(meshList[GEO_HUMAN_BODY_LOW], meshList[GEO_HUMAN_BODY_MED], meshList[GEO_HUMAN_BODY_HIGH]);
	m_cSceneGraph->AddChild(bomber);
}

void GameScene::bomberSurvivalInit(unsigned left, unsigned right, unsigned top, unsigned bot)
{
	// Set this to normal survival mode
	m_demoMode = false;

	const float BUFFER_SPACE = 25.0f;
	float worldWidth = m_cSpatialPartition->GetGridSizeX() * m_cSpatialPartition->GetxNumOfGrid() - BUFFER_SPACE;
	float worldHeight = m_cSpatialPartition->GetGridSizeY() * m_cSpatialPartition->GetyNumOfGrid() - BUFFER_SPACE;
	Bomber* bomber = nullptr;

	// Adding Bombers to the left side of the Scene
	for (size_t bots = 0; bots < left; ++bots)
	{
		bomber = new Bomber;
		bomber->Init(Vector3(Math::RandFloatMinMax(BUFFER_SPACE, worldWidth), 0, BUFFER_SPACE), meshList[GEO_HUMAN_HAT], meshList[GEO_HUMAN_HEAD], meshList[GEO_HUMAN_BODY_LOW]);
		bomber->SetBodyLODModels(meshList[GEO_HUMAN_BODY_LOW], meshList[GEO_HUMAN_BODY_MED], meshList[GEO_HUMAN_BODY_HIGH]);
		bomber->SetFlock(&m_bomberList);
		m_cSceneGraph->AddChild(bomber);
		m_bomberList.push_back(bomber);
	}

	// Adding Bombers to the right side of the Scene
	for (size_t bots = 0; bots < right; ++bots)
	{
		bomber = new Bomber;
		bomber->Init(Vector3(Math::RandFloatMinMax(BUFFER_SPACE, worldWidth), 0, worldHeight), meshList[GEO_HUMAN_HAT], meshList[GEO_HUMAN_HEAD], meshList[GEO_HUMAN_BODY_LOW]);
		bomber->SetBodyLODModels(meshList[GEO_HUMAN_BODY_LOW], meshList[GEO_HUMAN_BODY_MED], meshList[GEO_HUMAN_BODY_HIGH]);
		bomber->SetFlock(&m_bomberList);
		m_cSceneGraph->AddChild(bomber);
		m_bomberList.push_back(bomber);
	}

	// Adding Bombers to the top side of the Scene
	for (size_t bots = 0; bots < top; ++bots)
	{
		bomber = new Bomber;
		bomber->Init(Vector3(worldWidth, 0, Math::RandFloatMinMax(BUFFER_SPACE, worldHeight)), meshList[GEO_HUMAN_HAT], meshList[GEO_HUMAN_HEAD], meshList[GEO_HUMAN_BODY_LOW]);
		bomber->SetBodyLODModels(meshList[GEO_HUMAN_BODY_LOW], meshList[GEO_HUMAN_BODY_MED], meshList[GEO_HUMAN_BODY_HIGH]);
		bomber->SetFlock(&m_bomberList);
		m_cSceneGraph->AddChild(bomber);
		m_bomberList.push_back(bomber);
	}

	// Adding Bombers to the bottom side of the Scene
	for (size_t bots = 0; bots < bot; ++bots)
	{
		bomber = new Bomber;
		bomber->Init(Vector3(BUFFER_SPACE, 0, Math::RandFloatMinMax(BUFFER_SPACE, worldHeight)), meshList[GEO_HUMAN_HAT], meshList[GEO_HUMAN_HEAD], meshList[GEO_HUMAN_BODY_LOW]);
		bomber->SetBodyLODModels(meshList[GEO_HUMAN_BODY_LOW], meshList[GEO_HUMAN_BODY_MED], meshList[GEO_HUMAN_BODY_HIGH]);
		bomber->SetFlock(&m_bomberList);
		m_cSceneGraph->AddChild(bomber);
		m_bomberList.push_back(bomber);
	}

	m_numEnemiesAtStart = left + right + top + bot;
}

int GameScene::getNumBombersAlive(void)
{
	int count = 0;

	for (auto bomb = m_bomberList.begin(); bomb != m_bomberList.end(); ++bomb)
	{
		if ((*bomb)->GetActive())
		{
			++count;
		}
	}

	return count;
}

void GameScene::killPlayer(void)
{
	// Remove a life
	--m_lives;
	// Start invuln so that the player can get his bearings
	m_invulnTime = INVULN_TIME;
}

void GameScene::checkEndState(double dt)
{
	// Check if game end
	if (!m_demoMode && getNumBombersAlive() == 0)
	{
		// Add score to highscore
		HighScoreSystem::AddRecord(HighScoreRecord(m_levelName, m_score));

		// Go to Win State
		ostringstream oss;
		oss << m_score;
		changeState(WinState::Instance(), false, oss.str());
	}

	// Check if no health
	if (m_lives <= 0)
	{
		// Add score to highscore
		HighScoreSystem::AddRecord(HighScoreRecord(m_levelName, m_score));

		// Go to Lose State
		ostringstream oss;
		oss << m_score;
		changeState(LoseState::Instance(), false, oss.str());
	}
}

void GameScene::renderUIBar(Vector3 pos, Vector3 scale, float progress, Mesh * progressMesh)
{
	float ammoBarLength = progress * scale.x;
	float barDisplaceLength = scale.x - ammoBarLength;
	Vector3 barScale = scale;
	barScale.x = ammoBarLength;

	// Rendering
	Render2DMesh(progressMesh, false, barScale.x, barScale.y, pos.x + barDisplaceLength * 0.5, pos.y);
	Render2DMesh(meshList[GEO_BAR_BG], false, scale.x, scale.y, pos.x, pos.y);
}

int GameScene::bomberSurvivalInit(lua_State* L)
{
	vector<Lua::LuaTypePtr> params;
	vector<Lua::LuaTypePtr> returns;
	/*LT_POINTER: Context | LT_NUMBER: Left | LT_NUMBER: Right | LT_NUMBER: Top | LT_NUMBER: Bottom*/
	LuaFunc::preCall(L, params, vector<Lua::Type::LUA_TYPE> { Type::LT_POINTER, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER });

	GameScene* context = (GameScene*)Lua::ExtPtr(params[0]);

	context->bomberSurvivalInit(Lua::ExtNum(params[1]), Lua::ExtNum(params[2]), Lua::ExtNum(params[3]), Lua::ExtNum(params[4]));

	return LuaFunc::postCall(L, returns);
}

int GameScene::loadMeshRay(lua_State * L)
{
	vector<Lua::LuaTypePtr> params;
	vector<Lua::LuaTypePtr> returns;
	/*LT_POINTER: Context | LT_STRING: Mesh Name | LT_NUMBER: Color - Red | LT_NUMBER: Color - Green | LT_NUMBER: Color - Blue | LT_NUMBER: Length*/
	LuaFunc::preCall(L, params, vector<Lua::Type::LUA_TYPE> { Type::LT_POINTER, Type::LT_STRING, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER });

	GameScene* context = (GameScene*)Lua::ExtPtr(params[0]);

	context->loadMeshRay(Lua::ExtStr(params[1]), Color(Lua::ExtNum(params[2]), Lua::ExtNum(params[3]), Lua::ExtNum(params[4])), Lua::ExtNum(params[5]));

	return LuaFunc::postCall(L, returns);
}

int GameScene::loadMeshQuad(lua_State * L)
{
	vector<Lua::LuaTypePtr> params;
	vector<Lua::LuaTypePtr> returns;
	/*LT_POINTER: Context | LT_STRING: Mesh Name | LT_NUMBER: Color - Red | LT_NUMBER: Color - Green | LT_NUMBER: Color - Blue | LT_NUMBER: Length*/
	LuaFunc::preCall(L, params, vector<Lua::Type::LUA_TYPE> { Type::LT_POINTER, Type::LT_STRING, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER });

	GameScene* context = (GameScene*)Lua::ExtPtr(params[0]);

	context->loadMeshQuad(Lua::ExtStr(params[1]), Color(Lua::ExtNum(params[2]), Lua::ExtNum(params[3]), Lua::ExtNum(params[4])), Lua::ExtNum(params[5]));

	return LuaFunc::postCall(L, returns);
}

int GameScene::loadMesh2DMesh(lua_State * L)
{
	vector<Lua::LuaTypePtr> params;
	vector<Lua::LuaTypePtr> returns;
	/*LT_POINTER: Context | LT_STRING: Mesh Name | LT_NUMBER: Color - Red | LT_NUMBER: Color - Green | LT_NUMBER: Color - Blue | 
	  LT_NUMBER: PosX | LT_NUMBER: PosY | LT_NUMBER: Width | LT_NUMBER: Height*/
	LuaFunc::preCall(L, params, vector<Lua::Type::LUA_TYPE> { Type::LT_POINTER, Type::LT_STRING, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER });

	GameScene* context = (GameScene*)Lua::ExtPtr(params[0]);

	context->loadMesh2DMesh(Lua::ExtStr(params[1]), Color(Lua::ExtNum(params[2]), Lua::ExtNum(params[3]), Lua::ExtNum(params[4])), Lua::ExtNum(params[5]), Lua::ExtNum(params[6]), Lua::ExtNum(params[7]), Lua::ExtNum(params[8]));

	return LuaFunc::postCall(L, returns);
}

int GameScene::loadMeshSphere(lua_State * L)
{
	vector<Lua::LuaTypePtr> params;
	vector<Lua::LuaTypePtr> returns;
	/*LT_POINTER: Context | LT_STRING: Mesh Name | LT_NUMBER: Color - Red | LT_NUMBER: Color - Green | LT_NUMBER: Color - Blue | LT_NUMBER: Stack | LT_NUMBER: Slice | LT_NUMBER: Radius*/
	LuaFunc::preCall(L, params, vector<Lua::Type::LUA_TYPE> { Type::LT_POINTER, Type::LT_STRING, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER });

	GameScene* context = (GameScene*)Lua::ExtPtr(params[0]);

	context->loadMeshSphere(Lua::ExtStr(params[1]), Color(Lua::ExtNum(params[2]), Lua::ExtNum(params[3]), Lua::ExtNum(params[4])), Lua::ExtNum(params[5]), Lua::ExtNum(params[6]), Lua::ExtNum(params[7]));

	return LuaFunc::postCall(L, returns);
}

int GameScene::loadMeshCone(lua_State * L)
{
	vector<Lua::LuaTypePtr> params;
	vector<Lua::LuaTypePtr> returns;
	/*LT_POINTER: Context | LT_STRING: Mesh Name | LT_NUMBER: Color - Red | LT_NUMBER: Color - Green | LT_NUMBER: Color - Blue | LT_NUMBER: Slice | LT_NUMBER: Radius | LT_NUMBER: Height*/
	LuaFunc::preCall(L, params, vector<Lua::Type::LUA_TYPE> { Type::LT_POINTER, Type::LT_STRING, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER });

	GameScene* context = (GameScene*)Lua::ExtPtr(params[0]);

	context->loadMeshCone(Lua::ExtStr(params[1]), Color(Lua::ExtNum(params[2]), Lua::ExtNum(params[3]), Lua::ExtNum(params[4])), Lua::ExtNum(params[5]), Lua::ExtNum(params[6]), Lua::ExtNum(params[7]));

	return LuaFunc::postCall(L, returns);
}

int GameScene::loadMeshCrosshair(lua_State * L)
{
	vector<Lua::LuaTypePtr> params;
	vector<Lua::LuaTypePtr> returns;
	/*LT_POINTER: Context | LT_STRING: Mesh Name | LT_NUMBER: Color - Red | LT_NUMBER: Color - Green | LT_NUMBER: Color - Blue | LT_NUMBER: Slice | LT_NUMBER: Radius | LT_NUMBER: Height*/
	LuaFunc::preCall(L, params, vector<Lua::Type::LUA_TYPE> { Type::LT_POINTER, Type::LT_STRING, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER, Type::LT_NUMBER });

	GameScene* context = (GameScene*)Lua::ExtPtr(params[0]);

	context->loadMeshCrosshair(Lua::ExtStr(params[1]), Color(Lua::ExtNum(params[2]), Lua::ExtNum(params[3]), Lua::ExtNum(params[4])), Lua::ExtNum(params[5]));

	return LuaFunc::postCall(L, returns);
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
	// Set up OSS for on screen text
	std::ostringstream ss;
	ss.precision(5);

	// Render scores in the top left
	ss << "Score: " << m_score;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(), 30, 0.0f, m_window_height - 30);
	ss.str("");

	// Render lives in top right
	static const float LIFE_SIZE = 50;
	for (size_t i = 1; i <= m_lives; ++i)
	{
		Render2DMesh(meshList[GEO_LIFE], false, LIFE_SIZE, LIFE_SIZE, m_window_width - (i * LIFE_SIZE), m_window_height - LIFE_SIZE);
	}

	// Render the crosshair
	RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 10.0f);

	// Render Ammo Bar
	const Vector3 MAX_BAR_SCALE = Vector3(300.0f, 30.0f, 5.0f);
	const Vector3 AMMO_BAR_POS = Vector3(m_window_width - MAX_BAR_SCALE.x * 0.5f, MAX_BAR_SCALE.y * 0.5f);
	if (m_slowGun.IsReloading())
	{
		// Show reload status
		renderUIBar(AMMO_BAR_POS, MAX_BAR_SCALE, m_slowGun.GetReloadStatus(), meshList[GEO_AMMO_BAR]);
	}
	else
	{
		// Show ammo left
		renderUIBar(AMMO_BAR_POS, MAX_BAR_SCALE, (static_cast<float>(m_slowGun.GetCurrentMag()) / m_slowGun.GetMagSize()), meshList[GEO_AMMO_BAR]);
	}

	// Render Power Ammo Bar
	const Vector3 PAMMO_BAR_POS = Vector3(m_window_width - MAX_BAR_SCALE.x * 0.5f, MAX_BAR_SCALE.y * 1.5f);
	// Show collection status
	renderUIBar(PAMMO_BAR_POS, MAX_BAR_SCALE, (static_cast<float>(m_lazerCollection) / static_cast<float>(LAZER_PRICE)), meshList[GEO_POWER_AMMO_BAR]);

	ss << m_killGun.GetCharge();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(), 30, m_window_width - ss.str().length() * 30, MAX_BAR_SCALE.y);
	ss.str("");
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
	for (int i = 0; i < m_cProjectileManager->GetMaxNumberOfProjectiles(); i++)
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
				glLineWidth(8.0f);
				modelStack.Rotate(m_cProjectileManager->theListOfProjectiles[i]->GetRotationZ(), 0, 1, 0);
				modelStack.Rotate(-m_cProjectileManager->theListOfProjectiles[i]->GetRotationY(), 0, 0, 1);
				RenderMesh(m_meshResource["Ray"], false);
				glLineWidth(1.0f);
			}
			modelStack.PopMatrix();
		}
	}

	// Render the Avatar
	modelStack.PushMatrix();
	modelStack.Translate(m_cAvatar->GetPos_x(), m_cAvatar->GetPos_y(), m_cAvatar->GetPos_z());
	modelStack.Rotate(m_cAvatar->GetYRotation(), 0, 1, 0);
	modelStack.Scale(m_cAvatar->GetScale().x, m_cAvatar->GetScale().y, m_cAvatar->GetScale().z);
	RenderMesh(m_cAvatar->theAvatarMesh, false);

		// Render the Head
		modelStack.PushMatrix();
		modelStack.Translate(0.0f, 0.6f, 0.0f);
		modelStack.Scale(0.4f, 0.4f, 0.4f);
		RenderMesh(meshList[GEO_HUMAN_HEAD], false);

			// Render the Hat
			modelStack.PushMatrix();
			modelStack.Translate(0.0f, 0.5f, 0.0f);
			modelStack.Scale(1.5f, 1.5f, 1.5f);

			if (m_invulnTime > 0.0f)
			{
				RenderMesh(meshList[GEO_PLAYER_INVULN_HAT], false);
			}
			else
			{
				RenderMesh(meshList[GEO_HUMAN_HAT], false);
			}
			
			modelStack.PopMatrix();

		modelStack.PopMatrix();


	modelStack.PopMatrix();
}

/********************************************************************************
Render the lights in this scene
********************************************************************************/
void GameScene::RenderFixedObjects()
{
	//RenderMesh(meshList[GEO_AXES], false);
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

		 //cout << "Rendering..." << endl;
		for (int i = 0; i<m_cSpatialPartition->GetxNumOfGrid(); i++)
		{
			for (int j = 0; j<m_cSpatialPartition->GetyNumOfGrid(); j++)
			{
				//cout << m_cSpatialPartition->GetGridItemSize(i, j) << endl;

				// Only render it if there is something in it
				if (m_cSpatialPartition->GetGridActiveItemCount(i, j) <= 0)
				{
					continue;
				}

				modelStack.PushMatrix();
				modelStack.Translate(m_cSpatialPartition->xGridSize*i, -m_cSpatialPartition->yGridSize*j, 0.0f);
				modelStack.Scale(m_cSpatialPartition->GetGridSizeX(), m_cSpatialPartition->GetGridSizeY(), 1);
				Mesh* t = m_cSpatialPartition->GetGridMesh(i, j);
				RenderMesh(t, false);
				modelStack.PopMatrix();
			}
		}
		//cout << "Rendering ENDS" << endl;
		modelStack.PopMatrix();
	}
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_cSpatialPartition->GetGridSizeX() * m_cSpatialPartition->GetxNumOfGrid()* 0.5f, -m_cSpatialPartition->GetGridSizeY() * m_cSpatialPartition->GetyNumOfGrid() * 0.5f, 0.0f);
		//modelStack.Rotate(-90, 0, 0, 1);
		modelStack.Scale(m_cSpatialPartition->GetGridSizeX() * m_cSpatialPartition->GetxNumOfGrid(), m_cSpatialPartition->GetGridSizeY() * m_cSpatialPartition->GetyNumOfGrid(), 1.0f);
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