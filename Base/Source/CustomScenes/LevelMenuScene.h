#pragma once

// Inheritance Include
#include "../CustomScenes/MenuScene.h"

class LevelMenuScene : public MenuScene
{
	enum GEOMETRY_TYPE
	{
		GEO_TITLE,
		GEO_BT_BACK,
		GEO_BT_LEVEL_DEMO,
		GEO_BT_LEVEL_1,
		GEO_BT_LEVEL_2,
		GEO_BT_LEVEL_3,
		GEO_BT_LEVEL_4,
		GEO_BT_LEVEL_5,
		GEO_BT_LEVEL_6,
		GEO_AXES,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

	static const int BUTTON_ROWS = 3;
	static const int BUTTON_COLS = 2;

	enum BUTTON_TYPE
	{
		BT_BACK,
		BT_LEVEL_DEMO,
		BT_LEVEL_SURVIVAL1,
		BT_LEVEL_SURVIVAL2,
		BT_LEVEL_SURVIVAL3,
		BT_LEVEL_SURVIVAL4,
		BT_LEVEL_SURVIVAL5,
		BT_LEVEL_SURVIVAL6,
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