#pragma once

// Defines a weapon with unlimited stored ammo and functions for using the weapon
class Weapon
{
private:
	static const float HALF_RELOAD_SPEED_MULTIPLIER;
private:
	// Bullet Speed
	float m_bulletSpeed;
	// Reload
	float m_fullReloadSpeed;
	float m_reloadSpeed;
	bool m_reloading;
	float m_reloadTimer;
	// Fire Rate
	float m_fireRate;
	float m_lastShotDT;
	// Magazine Size
	short m_magSize;
	short m_currentMag;
	
public:
	Weapon();
	~Weapon();

	// Class Lifetime
	void Init(float bulletSpeed, float fullReloadSpeed, float fireRate, short magSize);
	void Update(double dt);

	// Actions
	bool Shoot(void);
	bool StartReload(void);

	// Get Reload Status: 0.0 - 1.0f: Percentage of reload completion
	float GetReloadStatus(void);
	float GetBulletSpeed(void);
};