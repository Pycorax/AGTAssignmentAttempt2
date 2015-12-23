#include "Bomber.h"

const float Bomber::DEATH_ROTATE_SPEED = 20.0f;
const float Bomber::DEATH_MAX_ROTATE = 30.0f;

Bomber::Bomber() : CSceneNode()
	, m_hat(nullptr)
	, m_head(nullptr)
	, m_body(nullptr)
	, m_speed(10.0f)
	, m_state(LS_CHASE)
	, m_deathRotated(0.0f)
	, m_bloated(0.0f)
{
}

Bomber::~Bomber()
{
}

void Bomber::Init(Vector3 startPos, Mesh* hatMesh, Mesh* headMesh, Mesh* bodyMesh)
{
	CTransform* rootPos = new CTransform(startPos.x, startPos.y, startPos.z);

	SetNode(rootPos, new CModel(bodyMesh));
	m_body = this;
	m_body->ApplyScale(8.0f, 8.0f, 8.0f);
	m_body->SetType(CSceneNode::NT_BOMBER);
	
	// Head
	CTransform* headPos = new CTransform(0.0f, 1.0f, 0.0f);
	m_head = new Bomber();
	m_head->SetNode(headPos, new CModel(headMesh));
	m_head->ApplyScale(0.4f, 0.4f, 0.4f);
	m_head->SetType(CSceneNode::NT_BOMBER);
	// Let the head know that THIS is the body
	m_head->m_body = this;
	// Add this to be the body
	this->AddChild(m_head);

	// Hat
	CTransform* hatPos = new CTransform(0.0f, 0.8f, 0.0f);
	m_hat = new Bomber();
	m_hat->SetNode(hatPos, new CModel(hatMesh));
	m_hat->ApplyScale(1.5f, 1.5f, 1.5f);
	m_hat->SetType(CSceneNode::NT_BOMBER);
	// Let the hat know that THIS is the body
	m_hat->m_body = this;
	// Add this to be the body
	m_head->AddChild(m_hat);
}

bool Bomber::Update(double dt, Vector3 target)
{
	static const float BOOM_RADIUS = 10.0f;
	static const float GROW_RADIUS = 50.0f;
	static const float MAX_BLOAT = 1.3f;
	static const float BLOAT_SPEED = 0.3f;

	if (GetActive() == false)
	{
		return false;
	}

	switch (m_state)
	{
		case LS_CHASE:
		{
			// Get a direction to the target
			Vector3 dir = target - GetTranslate();
			// Discard the y. We don't want to compare height.
			dir.y = 0;

			// Calculate dist to target
			float distToTargetSquared = dir.LengthSquared();

			// If close enough, explode
			if (distToTargetSquared < BOOM_RADIUS * BOOM_RADIUS)
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
				ApplyTranslate(move.x, move.y, move.z);
			}

			// Bloat up if near to player
			float bloat = 0;
			if (distToTargetSquared < GROW_RADIUS * GROW_RADIUS && m_bloated < MAX_BLOAT)
			{
				bloat = BLOAT_SPEED * dt;
				m_bloated += bloat;
				ApplyScale(1 + bloat, 1 + bloat, 1 + bloat);
			}
			else if (m_bloated > 0.0f)
			{
				bloat = BLOAT_SPEED * dt;
				m_bloated -= bloat;
				ApplyScale(1 - bloat, 1 - bloat, 1 - bloat);
			}
		}
		break;

		case LS_BOOM:
		{
			Deactivate();
			return true;
		}

		case LS_DEATH:
		{
			// Do death animation
			if (m_deathRotated < DEATH_MAX_ROTATE)
			{
				ApplyRotate(DEATH_ROTATE_SPEED * dt, 1, 0, 0);
				m_deathRotated += DEATH_ROTATE_SPEED * dt;
			}
			else
			{
				Deactivate();
			}
		}

		break;

	}

	return false;
}

void Bomber::Spawn(Vector3 startPos, float speed)
{
	SetTranslate(startPos.x, startPos.y, startPos.z);
	m_speed = speed;
	m_state = LS_CHASE;
	Activate();
}

void Bomber::Kill(void)
{
	m_deathRotated = 0.0f;
	m_state = LS_DEATH;
}

void Bomber::Nudge(Vector3 direction)
{
	if (direction != Vector3::ZERO_VECTOR)
	{
		// Discard the y
		direction.y = 0.0f;

		// Get the direction unit vector
		direction.Normalize();

		// Get the actual distance to move
		Vector3 move = direction * 5.0f;

		// Move in that direction
		ApplyTranslate(move.x, move.y, move.z);
	}
}

Bomber * Bomber::GetParent(void) const
{
	return m_body;
}
