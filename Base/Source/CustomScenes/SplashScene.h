#pragma once

// Inheritance Include
#include "../SceneManager.h"

class SplashScene : public CSceneManager
{
	static const float s_m_LIGHT_OFFSET;
	static const float s_m_LIGHT_SPEED;
	static const float s_m_END_TIME_PAUSE;

	float m_timePause;

	enum GEOMETRY_TYPE
	{
		GEO_LOGO,
		GEO_AXES,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

	// Render the main components of this scene
	void RenderGUI();
	void RenderFixedObjects();

public:
	SplashScene();
	SplashScene(const int window_width, const int window_height);
	~SplashScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	Mesh* meshList[NUM_GEOMETRY];
};