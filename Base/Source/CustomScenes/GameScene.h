#pragma once

// Inheritance Include
#include "../SceneManager.h"
#include "../Gameplay/BomberSpawner.h"

class GameScene : public CSceneManager
{
private:	// Static Constants
	static const int MAX_LIVES = 3;
	static const float INVULN_TIME;

private:	// Enums
	enum GEOMETRY_TYPE
	{
		GEO_YELLOW_CUBE,
		GEO_RAY,
		GEO_AXES,
		GEO_CROSSHAIR,
		GEO_LIGHTBALL,
		GEO_SPHERE,
		GEO_QUAD,
		GEO_CUBE,
		GEO_CONE,
		// -- Player
		GEO_PLAYER,
		// -- Human
		GEO_HUMAN_HAT,
		GEO_HUMAN_HEAD,
		GEO_HUMAN_BODY,
		// -- Skybox
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		// -- Floor
		GEO_GROUND,
		// -- HUD
		GEO_LIFE,
		// -- Text
		GEO_TEXT,
		NUM_GEOMETRY,
	};

private:	// Variables
	// Mesh Resources
	Mesh* meshList[NUM_GEOMETRY];

	// A list of bombers for players to shoot at
	vector<Bomber*> m_bomberList;

	// The moving bomber
	Bomber* m_movingBomber;

	// Number of Lives
	int m_lives;

	// Invulnerability Timer
	float m_invulnTime;

protected:
	// For Pausing and Resuming
	void OnResume();

	// Render the main components of this scene
	void RenderGUI();
	void RenderMobileObjects();
	void RenderFixedObjects();
	void RenderGround();
	void RenderSkybox();

public:
	GameScene();
	GameScene(const int window_width, const int window_height);
	~GameScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	virtual void UpdateWeaponStatus(const unsigned char key);

private:
	void meshInit();
	void bomberDemoInit();

	// Survival Mode
	// -- Bomber Functions
	void bomberSurvivalInit(unsigned left, unsigned right, unsigned top, unsigned bot);
	int getNumBombersAlive(void);
	// -- Player Functions
	void killPlayer(void);
	// -- Update Functions
	void checkEndState(double dt);
};