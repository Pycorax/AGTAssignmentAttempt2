#include "PowerWeapon.h"

PowerWeapon::PowerWeapon() : Weapon()
	, m_charges(0)
{
}

PowerWeapon::~PowerWeapon()
{
}

void PowerWeapon::Init(float bulletSpeed, float fullReloadSpeed, float fireRate, short magSize)
{
	Weapon::Init(bulletSpeed, fullReloadSpeed, fireRate, magSize);

	// Start with nothing
	m_currentMag = 0;
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
	InstantReload();
}

int PowerWeapon::GetCharge(void) const
{
	return m_charges;
}
