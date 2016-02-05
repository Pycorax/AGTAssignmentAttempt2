#pragma once

#include "Vector3.h"
#include "GL\glew.h"
#include "..\Mesh.h"
#include "Mtx44.h"

class CModel
{
public:
	enum RESOLUTION_TYPE
	{
		RT_LOW,
		RT_MED,
		RT_HIGH,
		RT_TOTAL
	};

	static const float LOD_LIMITS[RT_TOTAL];

private:
	Vector3 vTopLeft, vBottomRight;
	float red, green, blue;
	
	// Handle to the Mesh
	Mesh* m_cModelMesh;
	// LOD Meshes
	Mesh* m_lodMeshes[RT_TOTAL];

	// Current resolution
	RESOLUTION_TYPE m_currentRes;

public:
	CModel(Mesh* mesh = nullptr);
	~CModel(void);

	void SetMesh(Mesh* mesh);
	void SetLODMesh(Mesh* lowRes, Mesh* medRes, Mesh* highRes);
	void Draw(bool m_bLight = false);
	void SetColor(const float red, const float green, const float blue);

	// Get the top left of the bounding box of the instance
	Vector3 GetTopLeft(void);
	// Get the bottomright of the bounding box of the instance
	Vector3 GetBottomRight(void);

	// Resolution
	RESOLUTION_TYPE GetResolution(void) const;
	void SetResolution(RESOLUTION_TYPE type);

};
