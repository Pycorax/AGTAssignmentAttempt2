#pragma once

// Inheritance Include
#include "../CustomScenes/MenuScene.h"

class HighScoreMenuScene : public MenuScene
{
	enum GEOMETRY_TYPE
	{
		GEO_BT_BACK,
		GEO_BT_RESET,
		GEO_AXES,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

	enum BUTTON_TYPE
	{
		BT_BACK,
		BT_RESET,
		BT_TOTAL
	};

public:
	HighScoreMenuScene(const int window_width = 800, const int window_height = 600);
	~HighScoreMenuScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void UpdateWeaponStatus(const unsigned char key);

private:
	Mesh* meshList[NUM_GEOMETRY];
};