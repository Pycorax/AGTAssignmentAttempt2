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

	// Life Time
	void Init(float bulletSpeed, float fullReloadSpeed, float fireRate, short magSize);

	// Actions
	bool StartReload(void);

	// Charges
	void AddCharge(int charges = 1);
	int GetCharge(void) const;
};