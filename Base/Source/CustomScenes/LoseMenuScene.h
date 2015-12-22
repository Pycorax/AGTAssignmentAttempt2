#pragma once

// Inheritance Include
#include "../CustomScenes/MenuScene.h"
#include "../CustomScenes/IScoreMenu.h"

class LoseMenuScene : public MenuScene, public IScoreMenu
{
	enum GEOMETRY_TYPE
	{
		GEO_TITLE,
		GEO_BT_RETRY,
		GEO_BT_QUIT,
		GEO_AXES,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

	enum BUTTON_TYPE
	{
		BT_RETRY,
		BT_QUIT,
		BT_TOTAL
	};

public:
	LoseMenuScene(const int window_width = 800, const int window_height = 600, int score = 0);
	~LoseMenuScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void UpdateWeaponStatus(const unsigned char key);

private:
	Mesh* meshList[NUM_GEOMETRY];
};