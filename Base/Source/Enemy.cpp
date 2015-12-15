#include "Enemy.h"
#include <iostream>

CEnemy::CEnemy(void)
	: theStrategy(NULL)
{
}

CEnemy::~CEnemy(void)
{
	if (theStrategy != NULL)
	{
		delete theStrategy;
		theStrategy = NULL;
	}
}

// Initialise this class instance
void CEnemy::Init(void)
{
	theENEMYPosition.x=0;
	theENEMYPosition.y=0;
}

// Set position x of the player
void CEnemy::SetPos_x(int pos_x)
{
	theENEMYPosition.x = pos_x;
}

// Set position y of the player
void CEnemy::SetPos_y(int pos_y)
{
	theENEMYPosition.y = pos_y;
}

// Set the destination of this enemy
void CEnemy::SetDestination(const int pos_x, const int pos_y)
{
	theDestination.x = pos_x;
	theDestination.y = pos_y;
	if (theStrategy != NULL)
	{
		theStrategy->SetDestination(theDestination.x, theDestination.y);
	}
}

// Get position x of the player
int CEnemy::GetPos_x(void)
{
	return theENEMYPosition.x;
}

// Get position y of the player
int CEnemy::GetPos_y(void)
{
	return theENEMYPosition.y;
}

// Set the destination of this enemy
int CEnemy::GetDestination_x(void)
{
	return theDestination.x;
}

// Set the destination of this enemy
int CEnemy::GetDestination_y(void)
{
	return theDestination.y;
}

// Set Animation Invert status of the player
void CEnemy::SetAnimationInvert(bool enemyAnimationInvert)
{
	this->enemyAnimationInvert = enemyAnimationInvert;
}
// Get Animation Invert status of the player
bool CEnemy::GetAnimationInvert(void)
{
	return enemyAnimationInvert;
}

// Set Animation Counter of the player
void CEnemy::SetAnimationCounter(int enemyAnimationCounter)
{
	this->enemyAnimationCounter = enemyAnimationCounter;
}
// Get Animation Counter of the player
int CEnemy::GetAnimationCounter(void)
{
	return enemyAnimationCounter;
}

/********************************************************************************
 Hero Update
 ********************************************************************************/
void CEnemy::Update(CMap* m_cMap)
{
	if (theStrategy != NULL)
	{
		theStrategy->Update();
		theStrategy->GetEnemyPosition( (theENEMYPosition.x), (theENEMYPosition.y) );
	}
}

/********************************************************************************
 Strategy
 ********************************************************************************/
void CEnemy::ChangeStrategy(CStrategy* theNewStrategy, bool bDelete)
{
	if (bDelete == true)
	{
		if (theStrategy != NULL)
		{
			delete theStrategy;
			theStrategy = NULL;
		}
	}

	theStrategy = theNewStrategy;
	if (theStrategy != NULL)
	{
		theStrategy->SetEnemyPosition(theENEMYPosition.x, theENEMYPosition.y);
	}
}
