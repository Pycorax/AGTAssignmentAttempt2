#include "Bomber.h"

Bomber::Bomber() 
	: m_sceneGraph(nullptr)
	, m_hat(nullptr)
	, m_head(nullptr)
	, m_body(nullptr)
	, m_speed(0.0f)
{
}

Bomber::~Bomber()
{
}

void Bomber::Init(Vector3 startPos, Mesh* hatMesh, Mesh* headMesh, Mesh* bodyMesh)
{
	CTransform* rootPos = new CTransform(startPos.x, startPos.y, startPos.z);

	m_sceneGraph = new CSceneNode();
	m_sceneGraph->SetNode(rootPos, new CModel(bodyMesh));
	m_body = m_sceneGraph;
	m_body->ApplyScale(8.0f, 8.0f, 8.0f);
	m_body->SetType(CSceneNode::NT_BOMBER);
	
	// Head
	CTransform* headPos = new CTransform(0.0f, 1.0f, 0.0f);
	int headID = m_sceneGraph->AddChild(headPos, new CModel(headMesh));
	m_head = m_sceneGraph->GetNode(headID);
	m_head->ApplyScale(0.4f, 0.4f, 0.4f);
	m_head->SetType(CSceneNode::NT_BOMBER);

	// Hat
	CTransform* hatPos = new CTransform(0.0f, 0.8f, 0.0f);
	int hatID = m_head->AddChild(hatPos, new CModel(hatMesh));
	m_hat = m_sceneGraph->GetNode(hatID);
	m_hat->ApplyScale(1.5f, 1.5f, 1.5f);
	m_head->SetType(CSceneNode::NT_BOMBER);
}

void Bomber::Update(double dt, Vector3 target)
{
	static const float BOOM_RADIUS = 2.0f;

	if (m_sceneGraph->GetActive() == false)
	{
		return;
	}

	switch (m_state)
	{
		case LS_CHASE:
		{
			// Get a direction to the target
			Vector3 currentPos = m_sceneGraph->GetTranslate();
			Vector3 dir = target - currentPos;
			// Discard the y. We don't want to compare height.
			dir.y = 0;

			// If close enough, explode
			if (dir.LengthSquared() < BOOM_RADIUS * BOOM_RADIUS)
			{
				m_state = LS_BOOM;
			}
			else // Else Go towards
			{
				// Get Direction
				dir.Normalize();
				// Get Movement Vector
				Vector3 move = dir * m_speed * dt;
				// Move towards
				m_sceneGraph->ApplyTranslate(move.x, move.y, move.z);
			}
		}
		break;

		case LS_BOOM:
		{

		}
		break;

	}
}

void Bomber::Spawn(Vector3 startPos, float speed)
{
	// m_sceneGraph->SetTranslate(startPos.x, startPos.y, startPos.z);
	// Caclulate the delta
	Vector3 deltaDist = startPos - m_sceneGraph->GetTranslate();
	// Go towards this point
	//m_sceneGraph->ApplyTranslate(deltaDist.x, deltaDist.y, deltaDist.z);
	m_speed = speed;
	m_state = LS_CHASE;
	m_sceneGraph->Activate();
}

void Bomber::Kill(void)
{
	m_sceneGraph->Deactivate();
}

bool Bomber::GetActive(void)
{
	return m_sceneGraph->GetActive();
}

CSceneNode * Bomber::GetSceneGraph(void)
{
	return m_sceneGraph;
}
