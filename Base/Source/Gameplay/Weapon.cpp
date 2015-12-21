#include "Weapon.h"

const float Weapon::HALF_RELOAD_SPEED_MULTIPLIER = 0.8f;

Weapon::Weapon()
	: m_bulletSpeed(0.0f)
	, m_fullReloadTime(0.0f)
	, m_reloadTime(0.0f)
	, m_reloading(false)
	, m_reloadTimer(0.0f)
	, m_fireRate(0.0f)
	, m_lastShotDT(0.0f)
	, m_magSize(0)
	, m_currentMag(0)
{
}

Weapon::~Weapon()
{
}

void Weapon::Init(float bulletSpeed, float fullReloadSpeed, float fireRate, short magSize)
{
	m_bulletSpeed = bulletSpeed;
	m_fullReloadTime = fullReloadSpeed;
	m_reloadTime = HALF_RELOAD_SPEED_MULTIPLIER * m_fullReloadTime;
	m_lastShotDT = m_fireRate = 60.0f /*Seconds in a minute*/ / fireRate;
	m_currentMag = m_magSize = magSize;
}

void Weapon::Update(double dt)
{
	m_lastShotDT += dt;

	if (m_reloading)
	{
		if (m_reloadTimer > 0.0f)
		{
			// Update the reload timer
			m_reloadTimer -= dt;
		}
		else
		{
			// Reload
			m_currentMag = m_magSize;
			m_reloading = false;
		}
	}
}

bool Weapon::Shoot(void)
{
	if (m_lastShotDT > m_fireRate)
	{
		if (m_currentMag > 0)
		{
			m_lastShotDT = 0.0f;
			--m_currentMag;

			return true;
		}
		else
		{
			// Automatic reloads
			StartReload();
		}
	}

	return false;
}

bool Weapon::StartReload(void)
{
	// Don't allow reload if mag is full or no more bullets or is already reloading
	if (m_currentMag == m_magSize || m_reloading)
	{
		return false;
	}
	else
	{
		m_reloading = true;
		if (m_currentMag <= 0)
		{
			m_reloadTimer = m_fullReloadTime;
		}
		else
		{
			m_reloadTimer = m_reloadTime;
		}
		return true;
	}
}

float Weapon::GetReloadStatus(void)
{
	return 1.0f - (m_reloadTimer / m_fullReloadTime);
}

float Weapon::GetBulletSpeed(void)
{
	return m_bulletSpeed;
}

short Weapon::GetCurrentMag(void)
{
	return m_currentMag;
}

short Weapon::GetMagSize(void)
{
	return m_magSize;
}

bool Weapon::IsReloading(void)
{
	return m_reloading;
}
