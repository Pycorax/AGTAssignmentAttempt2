#pragma once

#include "Goodies.h"
#include "TreasureChest.h"

typedef int GoodiesID;
#define TREASURECHEST 100

class CGoodiesFactory
{
public:
	CGoodiesFactory(void);
	~CGoodiesFactory(void);

	CGoodies* Create(GoodiesID);
};
