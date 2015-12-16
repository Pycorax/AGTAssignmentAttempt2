#pragma once

// Inheritance Include
#include "../CustomScenes/MenuScene.h"

class LevelMenuScene : public MenuScene
{
	enum GEOMETRY_TYPE
	{
		GEO_TITLE,
		GEO_BT_BACK,
		GEO_BT_LEVEL,
		GEO_AXES,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

	static const int BUTTON_ROWS = 4;
	static const int BUTTON_COLS = 2;

	enum BUTTON_TYPE
	{
		BT_BACK,
		BT_LEVEL,
		BT_LEVEL_4_3 = BT_LEVEL + BUTTON_ROWS * BUTTON_COLS - 1,	// Create 12 Level buttons
		BT_TOTAL
	};

public:
	LevelMenuScene(const int window_width = 800, const int window_height = 600);
	~LevelMenuScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void UpdateWeaponStatus(const unsigned char key);

private:
	Mesh* meshList[NUM_GEOMETRY];
};