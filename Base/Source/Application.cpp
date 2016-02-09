#include "Application.h"

//Include GLEW
#include <GL/glew.h>
//Include GLFW
#include <GLFW/glfw3.h>

// STL Includes
#include <stdio.h>
#include <stdlib.h>

// API Includes
#include "SoundEngine.h"
#include "Lua.h"

// Other Includes
#include "GameState.h"
#include "CustomStates/introstate.h"
#include "Highscore\HighscoreSystem.h"
#include "CustomStates/playstate.h"

// Using Directives
using Lua::LuaFile;

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of thm_window_widthis game
const unsigned int frameTime = 1000 / FPS; // time for each frame
double Application::mouse_last_x = 0.0, Application::mouse_last_y = 0.0, 
	   Application::mouse_current_x = 0.0, Application::mouse_current_y = 0.0,
	   Application::mouse_diff_x = 0.0, Application::mouse_diff_y = 0.0;
double Application::camera_yaw = 0.0, Application::camera_pitch = 0.0;
int Application::s_window_width = 1280;
int Application::s_window_height = 800;
/********************************************************************************
 Define an error callback
 ********************************************************************************/
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

/********************************************************************************
 Define the key input callback
 ********************************************************************************/
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

/********************************************************************************
 Callback function when the window is resized
 ********************************************************************************/
void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

SoundPlayer2D* Application::m_bgm = nullptr;
bool Application::m_bgmPlaying = false;

/********************************************************************************
 Get keyboard's key states
 ********************************************************************************/
bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

/********************************************************************************
 Get mouse updates
 ********************************************************************************/
bool Application::GetMouseUpdate()
{
    glfwGetCursorPos(m_window, &mouse_current_x, &mouse_current_y);

	// Calculate the difference in positions
	mouse_diff_x = mouse_current_x - mouse_last_x;
	mouse_diff_y = mouse_current_y - mouse_last_y;

	//Calculate the yaw and pitch
	camera_yaw = (float) mouse_diff_x * 0.0174555555555556f;// * 3.142f / 180.0f;
	camera_pitch = mouse_diff_y * 0.0174555555555556f;// 3.142f / 180.0f );

	if (m_mouseHidden)
	{
		// Do a wraparound if the mouse cursor has gone out of the deadzone
		if ((mouse_current_x < m_window_deadzone) || (mouse_current_x > s_window_width - m_window_deadzone))
		{
			mouse_current_x = s_window_width >> 1;
			glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
		}
		if ((mouse_current_y < m_window_deadzone) || (mouse_current_y > s_window_height - m_window_deadzone))
		{
			mouse_current_y = s_window_height >> 1;
			glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
		}
	}

	// Store the current position as the last position
	mouse_last_x = mouse_current_x;
	mouse_last_y = mouse_current_y;

	// Get the mouse button status
	Button_Left   = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT);
	Button_Middle = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_MIDDLE);
	Button_Right  = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT);;

	// Update the GSM
	theGSM->HandleEvents( mouse_current_x, mouse_current_x, Button_Left, Button_Middle, Button_Right);

    return false;
}

/********************************************************************************
 Get keyboard updates
 ********************************************************************************/
bool Application::GetKeyboardUpdate()
{
	if (IsKeyPressed('A'))
	{
		theGSM->HandleEvents('a');
	}
	else
	{
		theGSM->HandleEvents('a', false);
	}
	if (IsKeyPressed('D'))
	{
		theGSM->HandleEvents('d');
	}
	else
	{
		theGSM->HandleEvents('d', false);
	}
	if (IsKeyPressed('W'))
	{
		theGSM->HandleEvents('w');
	}
	else
	{
		theGSM->HandleEvents('w', false);
	}
	if (IsKeyPressed('S'))
	{
		theGSM->HandleEvents('s');
	}
	else
	{
		theGSM->HandleEvents('s', false);
	}
	// Jump
	if (IsKeyPressed(32))
	{
		theGSM->HandleEvents(32);
	}

	// Escape Key
	if (IsKeyPressed(27))
	{
		theGSM->HandleEvents(27);
	}

	// Rotate camera
	if (IsKeyPressed(VK_LEFT))
	{
		theGSM->HandleEvents(VK_LEFT);
	}
	else
	{
		theGSM->HandleEvents(VK_LEFT, false);
	}
	if (IsKeyPressed(VK_RIGHT))
	{
		theGSM->HandleEvents(VK_RIGHT);
	}
	else
	{
		theGSM->HandleEvents(VK_RIGHT, false);
	}
	if (IsKeyPressed(VK_UP))
	{
		theGSM->HandleEvents(VK_UP);
	}
	else
	{
		theGSM->HandleEvents(VK_UP, false);
	}
	if (IsKeyPressed(VK_DOWN))
	{
		theGSM->HandleEvents(VK_DOWN);
	}
	else
	{
		theGSM->HandleEvents(VK_DOWN, false);
	}
	if (IsKeyPressed(VK_SHIFT))
	{
		theGSM->HandleEvents(VK_SHIFT);
	}
	else
	{
		theGSM->HandleEvents(VK_SHIFT, false);
	}
	return true;
}

void Application::SetCursorShown()
{
	// Show the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	GetInstance().setCursorShown();
}

void Application::SetCursorHidden()
{
	// Hide the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	GetInstance().setCursorHidden();
}

/********************************************************************************
 Constructor
 ********************************************************************************/
Application::Application()
	: theGSM(NULL)
	, m_mouseHidden(true)
{
}

/********************************************************************************
 Destructor
 ********************************************************************************/
Application::~Application()
{
	if (theGSM)
	{
		delete theGSM;
		theGSM = NULL;
	}
}

/********************************************************************************
 Initialise this program
 ********************************************************************************/
void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	// Load Game Properties from Lua
	LuaFile luaFile("Source//GameScripts//gameSettings.lua");
	s_window_width = luaFile.GetNumber("WINDOW_WIDTH");
	s_window_height = luaFile.GetNumber("WINDOW_HEIGHT");

	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(s_window_width, s_window_height, luaFile.GetString("WINDOW_TITLE").c_str(), NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	// Show the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set these 2 variables to zero
	m_dElapsedTime = 0.0;
	m_dAccumulatedTime_ThreadOne = 0.0;
	m_dAccumulatedTime_ThreadTwo = 0.0;

	// Initialise the GSM
	theGSM = new CGameStateManager();
	theGSM->Init( "DM2240 Assignment 1", s_window_width, s_window_height);
	//theGSM->ChangeState( CPlayState::Instance() );
	theGSM->ChangeState(CIntroState::Instance());

	// Start the high score system
	HighScoreSystem::Init();

	// Start the sound engine
	SoundEngine::StartSoundEngine();
	m_bgm = SoundEngine::CreateSound2D(SoundEngine::AddSoundSource("Sound//bgm.mp3"));
	StartSound();
}

/********************************************************************************
 Run this program
 ********************************************************************************/
void Application::Run()
{
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && theGSM->Running())
	{
		// Get the elapsed time
		m_dElapsedTime = m_timer.getElapsedTime();
		m_dAccumulatedTime_ThreadOne += m_dElapsedTime;
		m_dAccumulatedTime_ThreadTwo += m_dElapsedTime;
		if (m_dAccumulatedTime_ThreadOne > 0.03)
		{
			// Get inputs
			GetMouseUpdate();
			GetKeyboardUpdate();

			// Update the GSM
			theGSM->HandleEvents();
			theGSM->Update(m_dElapsedTime);

			m_dAccumulatedTime_ThreadOne = 0.0;
		}
		if (m_dAccumulatedTime_ThreadTwo > 1.0)
		{
			//UpdateAI();
			m_dAccumulatedTime_ThreadTwo = 0.0;
		}
		// Render the scene
		theGSM->Draw();
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

	} //Check if the ESC key had been pressed or if the window had been closed

	// Clean up the GSM
	theGSM->Cleanup();
}

/********************************************************************************
 Exit this program
 ********************************************************************************/
void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
	// Stop the sound engine
	SoundEngine::StopSoundEngine();
}

void Application::setCursorShown()
{
	// Show the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	m_mouseHidden = false;
}

void Application::setCursorHidden()
{
	// Hide the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	m_mouseHidden = true;
}

void Application::StartSound(void)
{
	m_bgm->Play(true);
	m_bgmPlaying = true;
}

void Application::StopSound(void)
{
	m_bgm->Pause();
	m_bgmPlaying = false;
}

bool Application::IsBGMPlaying(void)
{
	return m_bgmPlaying;
}
