#pragma once

// Inheritance Include
#include "../CustomScenes/MenuScene.h"

class MainMenuScene : public MenuScene
{
	enum GEOMETRY_TYPE
	{
		GEO_BG,
		GEO_BT_START,
		GEO_BT_OPTIONS,
		GEO_BT_INSTRUCTIONS,
		GEO_BT_HIGH_SCORE,
		GEO_BT_QUIT,
		GEO_AXES,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

	enum BUTTON_TYPE
	{
		BT_START,
		BT_INSTRUCTIONS,
		BT_HIGH_SCORE,
		BT_OPTIONS,
		BT_QUIT,
		BT_TOTAL
	};

public:
	MainMenuScene(const int window_width = 800, const int window_height = 600);
	~MainMenuScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void UpdateWeaponStatus(const unsigned char key);

private:
	Mesh* meshList[NUM_GEOMETRY];
};