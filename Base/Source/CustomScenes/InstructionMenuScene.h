#pragma once

// Inheritance Include
#include "../CustomScenes/MenuScene.h"

class InstructionMenuScene : public MenuScene
{
	enum GEOMETRY_TYPE
	{
		GEO_BG,
		GEO_TITLE,
		GEO_BT_BACK,
		GEO_AXES,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

	enum BUTTON_TYPE
	{
		BT_BACK,
		BT_TOTAL
	};

public:
	InstructionMenuScene(const int window_width = 800, const int window_height = 600);
	~InstructionMenuScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void UpdateWeaponStatus(const unsigned char key);

private:
	Mesh* meshList[NUM_GEOMETRY];
};