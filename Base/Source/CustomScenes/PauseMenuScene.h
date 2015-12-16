#pragma once

// Inheritance Include
#include "../CustomScenes/MenuScene.h"

class PauseMenuScene : public MenuScene
{
	enum GEOMETRY_TYPE
	{
		GEO_BT_BACK,
		GEO_BT_OPTIONS,
		GEO_BT_QUIT,
		GEO_AXES,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

	enum BUTTON_TYPE
	{
		BT_BACK,
		BT_OPTIONS,
		BT_QUIT,
		BT_TOTAL
	};

public:
	PauseMenuScene(const int window_width = 800, const int window_height = 600);
	~PauseMenuScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void UpdateWeaponStatus(const unsigned char key);

private:
	Mesh* meshList[NUM_GEOMETRY];
};