#pragma once

// Inheritance Include
#include "../SceneManager.h"

// Other Includes
#include "../Gameplay/Bomber.h"
#include "../Gameplay/PowerWeapon.h"

class GameScene : public CSceneManager
{
private:	// Static Constants
	// -- Gameplay
	static const int MAX_LIVES = 3;
	static const float INVULN_TIME;
	static const int LAZER_PRICE = 10;
	// -- Score
	static const int SCORE_FOR_HIT = 10;
	static const int SCORE_FOR_KILL = 100;

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
		GEO_PLAYER_BODY,
		GEO_PLAYER_INVULN_HAT,
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
		GEO_BAR_BG,
		GEO_AMMO_BAR,
		GEO_POWER_AMMO_BAR,
		GEO_KILL_BAR,
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

	// Gameplay Stats
	int m_numEnemiesAtStart;		// Stores the number of enemies at the start
	bool m_demoMode;				// The game mode

	// Number of Lives
	int m_lives;

	// Invulnerability Timer
	float m_invulnTime;

	// The Weapons the Player can use
	Weapon m_slowGun;
	PowerWeapon m_killGun;

	// Power Collection
	int m_lazerCollection;			// When more than LAZER_PRICE is collected, a charge is given to the lazer

	// Total Score
	int m_score;

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

	virtual void Init(bool demoMode, string levelString);
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

	// UI
	void renderUIBar(Vector3 pos, Vector3 scale, float progress, Mesh* progressMesh);
};