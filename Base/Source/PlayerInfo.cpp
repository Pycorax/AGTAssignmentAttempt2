#include "PlayerInfo.h"
#include <iostream>

CPlayerInfo::CPlayerInfo(void)
	: hero_inMidAir_Up(false)
	, hero_inMidAir_Down(false)
	, jumpspeed(0)
	, heroAnimationCounter(0)
	, heroAnimationInvert(false)
	, mapOffset_x(0)
	, mapOffset_y(0)
	, mapFineOffset_x(0)
	, mapFineOffset_y(0)
{
}

CPlayerInfo::~CPlayerInfo(void)
{
}

// Initialise this class instance
void CPlayerInfo::Init(void)
{
	theHeroPosition.x=0;
	theHeroPosition.y=0;
}

// Returns true if the player is on ground
bool CPlayerInfo::isOnGround(void)
{
	if (hero_inMidAir_Up == false && hero_inMidAir_Down == false)
		return true;

	return false;
}

// Returns true if the player is jumping upwards
bool CPlayerInfo::isJumpUpwards(void)
{
	if (hero_inMidAir_Up == true && hero_inMidAir_Down == false)
		return true;

	return false;
}

// Returns true if the player is on freefall
bool CPlayerInfo::isFreeFall(void)
{
	if (hero_inMidAir_Up == false && hero_inMidAir_Down == true)
		return true;

	return false;
}

// Set the player's status to free fall mode
void CPlayerInfo::SetOnFreeFall(bool isOnFreeFall)
{
	if (isOnFreeFall == true)
	{
		hero_inMidAir_Up = false;
		hero_inMidAir_Down = true;
		jumpspeed = 0;
	}
}

// Set the player to jumping upwards
void CPlayerInfo::SetToJumpUpwards(bool isOnJumpUpwards)
{
	if (isOnJumpUpwards == true)
	{
		hero_inMidAir_Up = true;
		hero_inMidAir_Down = false;
		jumpspeed = 15;
	}
}

// Set position x of the player
void CPlayerInfo::SetPos_x(int pos_x)
{
	theHeroPosition.x = pos_x;
}

// Set position y of the player
void CPlayerInfo::SetPos_y(int pos_y)
{
	theHeroPosition.y = pos_y;
}

// Set Jumpspeed of the player
void CPlayerInfo::SetJumpspeed(int jumpspeed)
{
	this->jumpspeed = jumpspeed;
}

// Stop the player's movement
void CPlayerInfo::SetToStop(void)
{
	hero_inMidAir_Up = false;
	hero_inMidAir_Down = false;
	jumpspeed = 0;
}

/********************************************************************************
 Hero Move Up Down
 ********************************************************************************/
void CPlayerInfo::MoveUpDown(const bool mode, const float timeDiff)
{
	if (mode)
	{
		theHeroPosition.y = theHeroPosition.y + (int) (5.0f * timeDiff);
	}
	else
	{
		theHeroPosition.y = theHeroPosition.y - (int) (5.0f * timeDiff);
	}
}

/********************************************************************************
 Hero Move Left Right
 ********************************************************************************/
void CPlayerInfo::MoveLeftRight(const bool mode, const float timeDiff)
{
	if (mode)
	{
		theHeroPosition.x = theHeroPosition.x - (int) (5.0f * timeDiff);
		heroAnimationInvert = true;
		heroAnimationCounter--;
		if (heroAnimationCounter==0)
			heroAnimationCounter = 3;
	}
	else
	{
		theHeroPosition.x = theHeroPosition.x + (int) (5.0f * timeDiff);
		heroAnimationInvert = false;
		heroAnimationCounter++;
		if (heroAnimationCounter>3)
			heroAnimationCounter = 0;
	}
}


// Get position x of the player
int CPlayerInfo::GetPos_x(void)
{
	return theHeroPosition.x;
}

// Get position y of the player
int CPlayerInfo::GetPos_y(void)
{
	return theHeroPosition.y;
}

// Get Jumpspeed of the player
int CPlayerInfo::GetJumpspeed(void)
{
	return jumpspeed;
}

// Get mapOffset_x
int CPlayerInfo::GetMapOffset_x(void)
{
	return mapOffset_x;
}
// Get mapOffset_y
int CPlayerInfo::GetMapOffset_y(void)
{
	return mapOffset_y;
}

// Get mapFineOffset_x
int CPlayerInfo::GetMapFineOffset_x(void)
{
	return mapFineOffset_x;
}
// Get mapFineOffset_y
int CPlayerInfo::GetMapFineOffset_y(void)
{
	return mapFineOffset_y;
}

// Update Jump Upwards
void CPlayerInfo::UpdateJumpUpwards()
{
	theHeroPosition.y -= jumpspeed;
	jumpspeed -= 1;
	if (jumpspeed == 0)
	{
		hero_inMidAir_Up = false;
		hero_inMidAir_Down = true;
	}
}

// Update FreeFall
void CPlayerInfo::UpdateFreeFall()
{
	theHeroPosition.y += jumpspeed;
	jumpspeed += 1;
}

// Set Animation Invert status of the player
void CPlayerInfo::SetAnimationInvert(bool heroAnimationInvert)
{
	this->heroAnimationInvert = heroAnimationInvert;
}
// Get Animation Invert status of the player
bool CPlayerInfo::GetAnimationInvert(void)
{
	return heroAnimationInvert;
}

// Set Animation Counter of the player
void CPlayerInfo::SetAnimationCounter(int heroAnimationCounter)
{
	this->heroAnimationCounter = heroAnimationCounter;
}
// Get Animation Counter of the player
int CPlayerInfo::GetAnimationCounter(void)
{
	return heroAnimationCounter;
}

// Constrain the position of the Hero to within the border
void CPlayerInfo::ConstrainHero(const int leftBorder, const int rightBorder, 
								  const int topBorder, const int bottomBorder, 
								  float timeDiff)
{
	if (theHeroPosition.x < leftBorder)
	{
		theHeroPosition.x = leftBorder;
		mapOffset_x =  mapOffset_x - (int) (5.0f * timeDiff);
		if (mapOffset_x < 0)
			mapOffset_x = 0;
	}
	else if (theHeroPosition.x > rightBorder)
	{
		theHeroPosition.x = rightBorder;
		mapOffset_x =  mapOffset_x + (int) (5.0f * timeDiff);
		if (mapOffset_x > 800)	// This must be changed to soft-coded
			mapOffset_x = 800;
	}

	if (theHeroPosition.y < topBorder)
		theHeroPosition.y = topBorder;
	else if (theHeroPosition.y > bottomBorder)
		theHeroPosition.y = bottomBorder;
}


/********************************************************************************
 Hero Update
 ********************************************************************************/
void CPlayerInfo::HeroUpdate(CMap* m_cMap)
{
	// Update Hero's info
  	if (hero_inMidAir_Up == false && hero_inMidAir_Down == false)
	{
		// Don't jump
	}
	else if (hero_inMidAir_Up == true && hero_inMidAir_Down == false)
	{
		// Check if the hero can move up into mid air...
		int checkPosition_X = (int) ((mapOffset_x+theHeroPosition.x) / m_cMap->GetTileSize());
		int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (theHeroPosition.y + m_cMap->GetTileSize() + jumpspeed) / m_cMap->GetTileSize());
		if ( (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 1) ||
		     (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == 1) )
		{
			// Since the new position does not allow the hero to move into, then go back to the old position
			theHeroPosition.y = ((int) (theHeroPosition.y / m_cMap->GetTileSize())) * m_cMap->GetTileSize();
			hero_inMidAir_Up = false;
			jumpspeed = 0;
		}
		else
		{
			theHeroPosition.y += jumpspeed;
			jumpspeed -= 1;
			if (jumpspeed == 0)
			{
				hero_inMidAir_Up = false;
				hero_inMidAir_Down = true;
			}
		}
	}
	else if (hero_inMidAir_Up == false && hero_inMidAir_Down == true)
	{
		// Check if the hero is still in mid air...
		int checkPosition_X = (int) ((mapOffset_x+theHeroPosition.x) / m_cMap->GetTileSize());
		if (checkPosition_X < 0)
			checkPosition_X = 0;
		if (checkPosition_X > m_cMap->getNumOfTiles_MapWidth())
			checkPosition_X = m_cMap->getNumOfTiles_MapWidth();
		int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (theHeroPosition.y - jumpspeed) / m_cMap->GetTileSize());
		if (checkPosition_Y < 0)
			checkPosition_Y = 0;
		if (checkPosition_Y > m_cMap->GetNumOfTiles_Height())
			checkPosition_Y = m_cMap->GetNumOfTiles_Height();
		if (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 1)
		{
			// Since the new position does not allow the hero to move into, then go back to the old position
			theHeroPosition.y = ((int) (theHeroPosition.y / m_cMap->GetTileSize())) * m_cMap->GetTileSize();
			hero_inMidAir_Down = false;
			jumpspeed = 0;
		}
		else
		{
			theHeroPosition.y -= jumpspeed;
			jumpspeed += 1;
		}
	}

	ConstrainHero(25, 750, 25, 575, 1.0f);

	// Calculate the fine offset
	mapFineOffset_x = mapOffset_x % m_cMap->GetTileSize();

}