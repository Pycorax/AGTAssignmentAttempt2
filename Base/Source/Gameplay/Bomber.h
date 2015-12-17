#pragma once

#include "..\SceneGraph\SceneNode.h"

class Bomber
{
public:
	enum LIFE_STATE
	{
		LS_CHASE,
		LS_BOOM,
		LS_TOTAL
	};

private:
	// SceneGraph for this Character
	CSceneNode* m_sceneGraph;

	// Body Parts
	CSceneNode* m_hat;
	CSceneNode* m_head;
	CSceneNode* m_body;

	// Human Stats
	float m_speed;
	LIFE_STATE m_state;

public:
	Bomber();
	virtual ~Bomber();

	void Init(Vector3 startPos, Mesh* hatMesh, Mesh* headMesh, Mesh* bodyMesh);
	void Update(double dt, Vector3 target);

	// Life Time
	void Spawn(Vector3 startPos, float speed);
	void Kill(void);

	// Check Status
	bool GetActive(void);

	// Get the graph for putting into the main scene graph
	CSceneNode* GetSceneGraph(void);
};
