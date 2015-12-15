#pragma once
#include "Strategy.h"
#include "Vector2.h"

class CStrategy_Kill
	: public CStrategy
{
public:
	CStrategy_Kill(void);
	~CStrategy_Kill(void);

	void Update(void);
	void SetDestination(const float x, const float y);
	void GetEnemyPosition(float& x, float& y);
	void SetEnemyPosition(const float x, const float y);

	// The AI states of the enemy
	enum CURRENT_STATE
	{
		IDLE,
		REPEL,
		ATTACK,
		NUM_ENEMY_STATE,
	};
	CStrategy_Kill::CURRENT_STATE GetState(void);
	void SetState(CStrategy_Kill::CURRENT_STATE);

private:
	//// The Destination is the position of the target
	//Vector2 theDestination;
	//// The position of the enemy
	//Vector2 theEnemyPosition;

	// Enemy AI State
	CStrategy_Kill::CURRENT_STATE CurrentState;
};
