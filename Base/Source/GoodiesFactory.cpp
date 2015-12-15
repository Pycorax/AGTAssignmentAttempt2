#include "GoodiesFactory.h"
#include <stdlib.h>

CGoodiesFactory::CGoodiesFactory(void)
{
}

CGoodiesFactory::~CGoodiesFactory(void)
{
}

CGoodies* CGoodiesFactory::Create(GoodiesID id)
{
	CGoodies* theNewGoodies = NULL;
	switch (id)
	{
	case TREASURECHEST:
		theNewGoodies = new CTreasureChest;
		break;
	default:
		break;
	}

	return theNewGoodies;
}