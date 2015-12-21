#include "PowerWeapon.h"

PowerWeapon::PowerWeapon() : Weapon()
	, m_charges(0)
{
}

PowerWeapon::~PowerWeapon()
{
}


bool PowerWeapon::StartReload(void)
{
	if (m_charges > 0)
	{
		// Remove 1 charge
		--m_charges;
		return Weapon::StartReload();
	}
	else
	{
		return false;
	}
}

void PowerWeapon::AddCharge(int charges)
{
	m_charges += charges;
}
