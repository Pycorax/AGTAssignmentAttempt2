#include "BomberSpawner.h"



BomberSpawner::BomberSpawner()
	: m_spawnTime(0.0)
	, m_lastSpawn(0.0)
	, m_minSpeed(0.0f)
	, m_maxSpeed(0.0f)
{
}


BomberSpawner::~BomberSpawner()
{
}

void BomberSpawner::Init(int maxEnemies, Mesh * hatMesh, Mesh * headMesh, Mesh * bodyMesh, float minSpeed, float maxSpeed, float spawnTime)
{
	m_minSpeed = minSpeed;
	m_maxSpeed = maxSpeed;
	m_spawnTime = spawnTime;

	// Initialize some
	for (size_t i = 0; i < maxEnemies; ++i)
	{
		Bomber* newEnemy = new Bomber;
		newEnemy->Init(Vector3(25, 0, 25), hatMesh, headMesh, bodyMesh);
		newEnemy->Deactivate();

		m_enemyList.push_back(newEnemy);
	}
}

void BomberSpawner::Update(double dt, Vector3 target, bool targetInvuln)
{
	// Update Spawn Timer
	m_lastSpawn += dt;

	// Spawner
	if (m_lastSpawn > m_spawnTime)
	{
		Spawn();
		m_lastSpawn = 0.0;
	}

	// Update all of them
	for (auto enemy = m_enemyList.begin(); enemy != m_enemyList.end(); ++enemy)
	{
		(*enemy)->Update(dt, target, targetInvuln);
	}
}

void BomberSpawner::Exit()
{
	// Delete all of them
	while (!m_enemyList.empty())
	{
		if (m_enemyList.back())
		{
			delete m_enemyList.back();
		}

		m_enemyList.pop_back();
	}
}

void BomberSpawner::AddSpawnPoint(SpawnBounds bound)
{
	m_spawnBounds.push_back(bound);
}

void BomberSpawner::Spawn(void)
{
	// Try to get hold of an enemy
	Bomber* newEnemy = getDeadEnemy();

	if (newEnemy)
	{
		// Randomly Select a Spawn Zone
		size_t spawnPoint = Math::RandIntMinMax(0, m_spawnBounds.size() - 1);

		// Randomly Select a point in that zone
		Vector3 spawnPos
			(
				Math::RandFloatMinMax(m_spawnBounds[spawnPoint].topLeft.x, m_spawnBounds[spawnPoint].botRight.x),
				Math::RandFloatMinMax(m_spawnBounds[spawnPoint].topLeft.y, m_spawnBounds[spawnPoint].botRight.y),
				Math::RandFloatMinMax(m_spawnBounds[spawnPoint].topLeft.z, m_spawnBounds[spawnPoint].botRight.z)
			);
		
		// Randomly Select a Speed
		float spawnSpeed = Math::RandFloatMinMax(m_minSpeed, m_maxSpeed);

		// Spawn It
		newEnemy->Spawn(spawnPos, spawnSpeed);
	}
	// Else don't do anything if can't find anything to spawn
}

vector<CSceneNode*> BomberSpawner::GetSceneNodes()
{
	vector<CSceneNode*> nodes;

	for (auto enemy = m_enemyList.begin(); enemy != m_enemyList.end(); ++enemy)
	{
		if ((*enemy)->GetActive() == false)
		{
			nodes.push_back(*enemy);
		}
	}

	return nodes;
}

Bomber * BomberSpawner::getDeadEnemy(void)
{
	for (auto enemy = m_enemyList.begin(); enemy != m_enemyList.end(); ++enemy)
	{
		if ((*enemy)->GetActive() == false)
		{
			return *enemy;
		}
	}

	return nullptr;
}
