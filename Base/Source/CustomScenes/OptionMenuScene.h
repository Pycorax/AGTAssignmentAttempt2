#pragma once

// Inheritance Include
#include "../CustomScenes/MenuScene.h"

class OptionMenuScene : public MenuScene
{
	enum GEOMETRY_TYPE
	{
		GEO_TITLE,
		GEO_BT_BACK,
		GEO_BT_MUSIC_ON,
		GEO_BT_MUSIC_OFF,
		GEO_AXES,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

	enum BUTTON_TYPE
	{
		BT_BACK,
		BT_MUSIC_TOGGLE,
		BT_TOTAL
	};

	Mesh* meshList[NUM_GEOMETRY];

public:
	OptionMenuScene(const int window_width = 800, const int window_height = 600);
	~OptionMenuScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void UpdateWeaponStatus(const unsigned char key);
};