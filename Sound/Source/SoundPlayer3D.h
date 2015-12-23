/******************************************************************************/
/*!
\file	SoundSource.h
\author Tng Kah Wei
\brief
Class to define audio from a specific point

This sound system is based on Irrklang and is a wrapper for Irrklang so that the
implementation in the Base project need not change should there be changes to
Irrklang's API
*/
/******************************************************************************/
#ifndef SOUND_POINT_H
#define SOUND_POINT_H

#include "SoundPlayer2D.h"
#include "Vector3.h"

using irrklang::vec3df;

class SoundPlayer3D : SoundPlayer2D
{
	friend class SoundEngine;

private:	// Variables
	Vector3 m_pos;

public:		// Functions
	void SetPos(Vector3 pos);

protected:	// Functions
	SoundPlayer3D(void);
	~SoundPlayer3D(void);

private:	// Functions
	void init(ISoundEngine* _engine, ISoundSource* _snd, Vector3 pos);

	// To convert from Vector3 to Irrklang's vec3df
	static vec3df toVec3df(Vector3 vec);
};

#endif