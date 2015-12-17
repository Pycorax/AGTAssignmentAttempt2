#include "Human.h"

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

void Human::Init(CTransform * rootPosition, Mesh* hatMesh, Mesh* headMesh, Mesh* bodyMesh)
{
	m_sceneGraph = new CSceneNode();
	m_sceneGraph->SetNode(rootPosition, new CModel(bodyMesh));
	m_body = m_sceneGraph;
	m_body->ApplyScale(8.0f, 8.0f, 8.0f);
	
	// Head
	CTransform* headPos = new CTransform(0.0f, 1.0f, 0.0f);
	int headID = m_sceneGraph->AddChild(headPos, new CModel(headMesh));
	m_head = m_sceneGraph->GetNode(headID);
	m_head->ApplyScale(0.4f, 0.4f, 0.4f);

	// Hat
	CTransform* hatPos = new CTransform(0.0f, 0.8f, 0.0f);
	int hatID = m_head->AddChild(hatPos, new CModel(hatMesh));
	m_hat = m_sceneGraph->GetNode(hatID);
	m_hat->ApplyScale(1.5f, 1.5f, 1.5f);
}

CSceneNode * Human::GetSceneGraph(void)
{
	return m_sceneGraph;
}
