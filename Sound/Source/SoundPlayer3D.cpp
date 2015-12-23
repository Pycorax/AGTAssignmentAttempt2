#include "SoundPlayer3D.h"

void SoundPlayer3D::SetPos(Vector3 pos)
{
	m_pos = pos;

	// Update the sound location in irrklang
	m__musicController->setPosition(toVec3df(m_pos));
}

SoundPlayer3D::SoundPlayer3D(void)
{
}

SoundPlayer3D::~SoundPlayer3D(void)
{
}

void SoundPlayer3D::init(ISoundEngine* _engine, ISoundSource* _snd, Vector3 pos)
{
	m__musicController = _engine->play3D(_snd, toVec3df(m_pos), false, true, true);
}

vec3df SoundPlayer3D::toVec3df(Vector3 vec)
{
	return vec3df(vec.x, vec.y, vec.z);
}