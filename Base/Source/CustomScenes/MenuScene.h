#pragma once

// Inheritance Include
#include "../SceneManager.h"

// Other Includes
#include "../UI/UIButton.h"

class MenuScene : public CSceneManager
{
	enum MOUSE_STATE_TYPE
	{
		UP_STATE,
		CLICK_DOWN_STATE,
		CLICK_HELD_STATE,
		CLICK_RELEASE_STATE,
		MS_TOTAL
	};

	// For Pausing and Resuming
	void OnResume();

	// Mouse Input
	bool m_rawLeftClick;
	MOUSE_STATE_TYPE m_leftMouseState;

	unsigned m_buttonSize;

	// Render the main components of this scene
	virtual void RenderGUI();

protected:
	// Background
	Mesh* m_bg;
	// Buttons
	UIButton* m_button;

public:
	MenuScene(const int window_width = 800, const int window_height = 600);
	virtual ~MenuScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	virtual void UpdateWeaponStatus(const unsigned char key);

protected:
	// Input Checking
	bool isMouseState(MOUSE_STATE_TYPE mouseState);

	// Create
	void createBackground(Mesh* bgMesh);
	// Create Button List
	void createButtonList(int sizeOfList);

private:
	// Updates
	void updateUIButtons(void);
	void mouseUpdate(void);

	// Renders
	void renderUIButton(UIButton btn);
};