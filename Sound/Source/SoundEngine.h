#ifndef SOUND_ENGINE_H
#define SOUND_ENGINE_H

#include "Irrklang\include\irrKlang.h"
#include "SoundPlayer2D.h"
#include "SoundPlayer3D.h"
#include "Vector3.h"
#include <vector>

#pragma comment(lib, "irrKlang.lib")

using std::string;
using std::vector;

using irrklang::ISoundEngine;
using irrklang::createIrrKlangDevice;
using irrklang::ISound;
using irrklang::ISoundSource;
using irrklang::vec3df;

class SoundEngine
{
protected:	// Variables
	static ISoundEngine* s__engine;
	static vector<ISoundSource*> s_soundLibrary;
	static vector<SoundPlayer2D*> s_soundPlayers;

public:		// Functions
	// For starting and stopping the s__engine
	static void StartSoundEngine(void);
	static void StopSoundEngine(void);

	// Sound Library
	static unsigned AddSoundSource(string filePath);
	static void ClearSoundLibrary(void);
	static void ClearSoundSources(void);
		
	// Sound Players
	static SoundPlayer2D* CreateSound2D(unsigned soundID);
	static SoundPlayer3D* CreateSound3D(unsigned soundID, Vector3 pos);

	static void StopAll(void);
};

#endif