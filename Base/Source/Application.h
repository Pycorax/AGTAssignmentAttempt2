#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "GameStateManager.h"
#include "SoundPlayer2D.h"

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}

	static bool IsKeyPressed(unsigned short key);
	bool GetMouseUpdate();
	bool GetKeyboardUpdate();

	// Cursor
	static void SetCursorShown(void);
	static void SetCursorHidden(void);

	void Init();
	void Run();
	void Exit();

	//Declare variables to store the last and current mouse position
	static double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	int Button_Left, Button_Middle, Button_Right;
	static double camera_yaw, camera_pitch;

	// BGM
	static SoundPlayer2D* m_bgm;
	static bool m_bgmPlaying;

private:
	Application();
	~Application();

	// Declare the window width and height as constant integer
	static const int m_window_deadzone = 100;
	static int s_window_width;
	static int s_window_height;
	//Declare a window object
	StopWatch m_timer;
	double m_dElapsedTime;
	double m_dAccumulatedTime_ThreadOne;
	double m_dAccumulatedTime_ThreadTwo;

	// Game State Management
	CGameStateManager* theGSM;

	// Hiding Mouse
	bool m_mouseHidden;

	void setCursorShown(void);
	void setCursorHidden(void);

public:
	// BGM
	static void StartSound(void);
	static void StopSound(void);
	static bool IsBGMPlaying(void);
};

#endif