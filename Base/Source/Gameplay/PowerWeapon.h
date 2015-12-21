#pragma once

// Inheritance Include
#include "Weapon.h"

// Variant of Weapon but has limited charges
class PowerWeapon : public Weapon
{
	int m_charges;
public:
	PowerWeapon();
	~PowerWeapon();

	// Actions
	bool StartReload(void);

	void AddCharge(int charges = 1);
};