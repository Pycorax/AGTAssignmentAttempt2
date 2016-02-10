#include "FlockState.h"

// Other Include
#include "../Bomber.h"

FlockState::FlockState()
{
}

FlockState::~FlockState()
{
}

void FlockState::Init(FiniteStateMachine * FSMOwner)
{
	State::Init(FSMOwner);
}

void FlockState::Init(State * stateOwner)
{
	State::Init(stateOwner);
}

void FlockState::Update(double dt)
{
	static const float COHESION_RADIUS = 8.0f;
	static const float SEPARATAION_RADIUS = 4.0f;
	static const float NEIGHBOURING_RADIUS = 20.0f;
	static const float NUDGE_SPEED = 2.5f;

	State::Update(dt);

	Bomber* bomber = dynamic_cast<Bomber*>(m_FSMOwner->GetParent());

	if (bomber)
	{
		struct Neighbour
		{
			Bomber *bomber;
			double dist;
			Vector3 pos;
		};

		vector<Neighbour> neighbours;
		Vector3 myPos;
		bomber->theTransform->GetOffset(myPos.x, myPos.y, myPos.z);

		// Identify Neighbours
		vector<Bomber*>* bomberList = bomber->m_flock;
		for (auto bomb : *bomberList)
		{
			// Obtain other's position
			Vector3 bombPos;
			bomb->theTransform->GetOffset(bombPos.x, bombPos.y, bombPos.z);

			// Calculate radius
			double distSquared = (myPos - bombPos).LengthSquared();
			if (distSquared < NEIGHBOURING_RADIUS * NEIGHBOURING_RADIUS)
			{
				Neighbour n;
				n.bomber = bomb;
				n.dist = distSquared;
				n.pos = bombPos;
				neighbours.push_back(n);
			}
		}

		// Calculate total nudging
		Vector3 totalNudge;
		for (auto neighbour : neighbours)
		{
			// Get Direction of the neighbour
			Vector3 dir = neighbour.pos - myPos;
			if (dir != Vector3::ZERO_VECTOR)
			{
				dir.Normalize();
			}

			if (neighbour.dist > COHESION_RADIUS * COHESION_RADIUS && neighbour.dist < SEPARATAION_RADIUS * SEPARATAION_RADIUS)
			{
				// Come Closer
				totalNudge += dir * NUDGE_SPEED * dt;
			}
			else if (neighbour.dist > SEPARATAION_RADIUS * SEPARATAION_RADIUS)
			{
				// Move away
				totalNudge -= dir * NUDGE_SPEED * dt;
			}
		}
		
		// Apply the nudging
		bomber->ApplyTranslate(totalNudge.x, totalNudge.y, totalNudge.z);
	}
}

void FlockState::Exit(void)
{
	State::Exit();
}
