#include "SoundPlayer2D.h"

using irrklang::ISoundSource;

void SoundPlayer2D::Play(bool loop)
{
	// Reload the Sound
	if (m__musicController->isFinished())
	{
		m__musicController->drop();
		m__musicController = NULL;
		m__musicController = m__parentEngine->play2D(m__soundSource, false, true, true);
	}

	m__musicController->setIsLooped(loop);
	m__musicController->setIsPaused(false);
}

void SoundPlayer2D::Stop(void)
{
	m__musicController->stop();
	m__musicController->drop();
	m__musicController = NULL;
}

void SoundPlayer2D::Pause(void)
{
	m__musicController->setIsPaused(true);
}

void SoundPlayer2D::SetVolume(float vol)
{
	// Clamp the vol provided
	if (vol < 0.0f)
	{
		vol = 0.0f;
	}
	else if (vol > 1.0f)
	{
		vol = 1.0f;
	}

	// Set the volume
	m__musicController->setVolume(vol);
}

SoundPlayer2D::SoundPlayer2D(void) : m__musicController(NULL)
{

}

SoundPlayer2D::~SoundPlayer2D(void)
{
}

void SoundPlayer2D::init(ISoundEngine* _engine, ISoundSource* _snd)
{
	// Store the sound for reloading if necessary
	m__soundSource = _snd;
	m__parentEngine = _engine;

	// Load the sound
	m__musicController = m__parentEngine->play2D(m__soundSource, false, true, true);
}