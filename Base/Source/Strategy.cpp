#include "Strategy.h"
#include <iostream>

using namespace std;

CStrategy::CStrategy()
{
}

CStrategy::~CStrategy()
{
}

void CStrategy::Update()
{
	cout << "void CStrategy::Update()" << endl;
}

int CStrategy::CalculateDistance(void)
{
	return ((theDestination.x - theEnemyPosition.x)*(theDestination.x - theEnemyPosition.x) + 
			(theDestination.y - theEnemyPosition.y)*(theDestination.y - theEnemyPosition.y));
}
