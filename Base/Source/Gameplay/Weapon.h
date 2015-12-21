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
	float m_fullReloadTime;
	float m_reloadTime;
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
	virtual ~Weapon();

	// Class Lifetime
	void Init(float bulletSpeed, float fullReloadSpeed, float fireRate, short magSize);
	virtual void Update(double dt);

	// Actions
	virtual bool Shoot(void);
	virtual bool StartReload(void);

	// Get Reload Status: 0.0 - 1.0f: Percentage of reload completion
	float GetReloadStatus(void);
	float GetBulletSpeed(void);
	short GetCurrentMag(void);
	short GetMagSize(void);

	bool IsReloading(void);
};