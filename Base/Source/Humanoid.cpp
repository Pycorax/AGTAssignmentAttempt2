#include "Humanoid.h"

Human::Human() 
	: m_sceneGraph(nullptr)
	, m_hat(nullptr)
	, m_head(nullptr)
	, m_body(nullptr)
{
}

Human::~Human()
{
}

void Human::Init(CTransform * rootPosition)
{
	m_sceneGraph = new CSceneNode();
	m_sceneGraph->SetNode(rootPosition, nullptr);
	
	// Head
	CTransform* headPos = new CTransform(0.0f, 1.0f, 0.0f);
	int headID = m_sceneGraph->AddChild(headPos, nullptr);
	m_head = m_sceneGraph->GetNode(headID);

	// Hat
	CTransform* hatPos = new CTransform(0.0f, 1.0f, 0.0f);
	int hatID = m_sceneGraph->AddChild(hatPos, nullptr);
	m_hat = m_sceneGraph->GetNode(hatID);
}
