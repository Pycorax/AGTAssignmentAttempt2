#pragma once

// Inheritance Include
#include "../SceneManager.h"

class GameScene : public CSceneManager
{
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
		GEO_GRASS_DARKGREEN,
		GEO_GRASS_LIGHTGREEN,
		// -- Text
		GEO_TEXT,
		NUM_GEOMETRY,
	};

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

private:
	Mesh* meshList[NUM_GEOMETRY];
	CSceneNode* mover;
};