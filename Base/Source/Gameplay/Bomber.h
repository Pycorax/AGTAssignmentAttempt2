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
	Bomber* m_hat;
	Bomber* m_head;
	Bomber* m_body;

	// Human Stats
	float m_speed;
	LIFE_STATE m_state;

public:
	Bomber();
	virtual ~Bomber();

	void Init(Vector3 startPos, Mesh* hatMesh, Mesh* headMesh, Mesh* bodyMesh);		// Should only be called on Body Bomber nodes
	bool Update(double dt, Vector3 target);

	// Life Time
	void Spawn(Vector3 startPos, float speed);

	// Interaction
	void Nudge(Vector3 direction);
	Bomber* GetParent(void) const;
};
