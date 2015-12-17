#pragma once

#include "Bomber.h"

struct SpawnBounds
{
	Vector3 topLeft;
	Vector3 botRight;

	SpawnBounds(Vector3 _topLeft, Vector3 _botRight)
		: topLeft( _topLeft)
		, botRight(_botRight)
	{	
	}
};

class BomberSpawner
{
	// Storage of Enemies
	vector<Bomber*> m_enemyList;
	
	// Spawning Conditions
	vector<SpawnBounds> m_spawnBounds;

	// Spawn Rate
	double m_spawnTime;
	double m_lastSpawn;

	// Speed
	float m_minSpeed;
	float m_maxSpeed;

public:
	BomberSpawner();
	~BomberSpawner();

	void Init(int maxEnemies, Mesh* hatMesh, Mesh* headMesh, Mesh* bodyMesh, float minSpeed = 5.0, float maxSpeed = 10.0, float spawnTime = 1.0f);
	void Update(double dt, Vector3 target);
	void Exit();

	// Spawning
	void AddSpawnPoint(SpawnBounds bound);
	void Spawn(void);

	// Adding to Scene Graph
	vector<CSceneNode*> GetSceneNodes();

private:
	Bomber* getDeadEnemy(void);
};

