#pragma once

// Inheritance Include
#include "..\SceneGraph\SceneNode.h"

class Bomber : public CSceneNode
{
public:
	enum LIFE_STATE
	{
		LS_CHASE,
		LS_BOOM,
		LS_TOTAL
	};

private:
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
};
