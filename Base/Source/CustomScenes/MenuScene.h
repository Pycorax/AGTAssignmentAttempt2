#pragma once

// Inheritance Include
#include "../SceneManager.h"

// Other Includes
#include "../UI/UIButton.h"

class MenuScene : public CSceneManager
{
	enum GEOMETRY_TYPE
	{
		GEO_BUTTON,
		GEO_AXES,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

	enum BUTTON_TYPE
	{
		BT_START,
		BT_TOTAL
	};

	enum MOUSE_STATE_TYPE
	{
		UP_STATE,
		CLICK_DOWN_STATE,
		CLICK_HELD_STATE,
		CLICK_RELEASE_STATE,
		MS_TOTAL
	};

	// Mouse Input
	bool m_rawLeftClick;
	MOUSE_STATE_TYPE m_leftMouseState;

	// Buttons
	UIButton m_button[BT_TOTAL];

	// Render the main components of this scene
	void RenderGUI();

public:
	MenuScene(const int window_width = 800, const int window_height = 600);
	~MenuScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void UpdateWeaponStatus(const unsigned char key);

protected:
	// Updates
	void mouseUpdate(void);
	void updateUIButtons(void);

	// Renders
	void renderUIButton(UIButton btn);

private:
	Mesh* meshList[NUM_GEOMETRY];
};