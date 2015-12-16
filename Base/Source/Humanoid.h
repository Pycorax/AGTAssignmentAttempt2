#pragma once

#include "SceneGraph\SceneNode.h"

class Human
{
private:
	// SceneGraph for this Character
	CSceneNode* m_sceneGraph;

	// Body Parts
	CSceneNode* m_hat;
	CSceneNode* m_head;
	CSceneNode* m_body;

public:
	Human();
	virtual ~Human();

	void Init(CTransform* rootPosition);
};
