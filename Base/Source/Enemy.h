#pragma once
#include "Vector2.h"
#include "Map.h"
#include "Strategy.h"

class CEnemy
{
//	friend class CStrategy;
public:
	CEnemy(void);
	~CEnemy(void);

	enum GEOMETRY_TYPE
	{
		GEO_TILEENEMY_FRAME0,
		GEO_TILEENEMY_FRAME1,
		GEO_TILEENEMY_FRAME2,
		GEO_TILEENEMY_FRAME3,
		NUM_GEOMETRY,
	};

	// Initialise this class instance
	void Init(void);
	// Set position x of the player
	void SetPos_x(int pos_x);
	// Set position y of the player
	void SetPos_y(int pos_y);
	// Set the destination of this enemy
	void SetDestination(const int pos_x, const int pos_y);

	// Get position x of the player
	int GetPos_x(void);
	// Get position y of the player
	int GetPos_y(void);
	// Set the destination of this enemy
	int GetDestination_x(void);
	// Set the destination of this enemy
	int GetDestination_y(void);

	// ENEMY Update
	void Update(CMap* m_cMap);

	// Strategy
	void ChangeStrategy(CStrategy* theNewStrategy, bool bDelete=true);

	// Set Animation Invert status of the player
	void SetAnimationInvert(bool ENEMYAnimationInvert);
	// Get Animation Invert status of the player
	bool GetAnimationInvert(void);
	// Set Animation Counter of the player
	void SetAnimationCounter(int ENEMYAnimationCounter);
	// Get Animation Counter of the player
	int GetAnimationCounter(void);

private:
	// ENEMY's information
	Vector2 theENEMYPosition;
	bool enemyAnimationInvert;
	int enemyAnimationCounter;

	// The Destination is the position of the Hero
	Vector2 theDestination;

	CStrategy* theStrategy;
};
