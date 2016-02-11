#include "Model.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#include <windows.h> // Header File For Windows
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library

#include <stdio.h>
#include "..\MeshBuilder.h"

const float CModel::LOD_LIMITS[RT_TOTAL] = { 0, 300, 150 };

CModel::CModel(Mesh* mesh)
	: m_cModelMesh(mesh)
	, m_currentRes(RT_HIGH)
{
	vTopLeft = Vector3( 0.5f, 0.5f, 0.5f );
	vBottomRight = Vector3( -0.5f, -0.5f, -0.5f );
	red = 128;
	green = 255;
	blue = 0;

	// Initialize the lod meshes
	for (size_t i = 0; i < RT_TOTAL; ++i)
	{
		m_lodMeshes[i] = nullptr;
	}
}

CModel::~CModel(void)
{

}

void CModel::SetMesh(Mesh* mesh)
{
	m_cModelMesh = mesh;
}

void CModel::SetLODMesh(Mesh * lowRes, Mesh * medRes, Mesh * highRes)
{
	m_lodMeshes[RT_LOW] = lowRes;
	m_lodMeshes[RT_MED] = medRes;
	m_lodMeshes[RT_HIGH] = highRes;
}

void CModel::Draw(bool m_bLight)
{
	if (m_lodMeshes[m_currentRes] != nullptr)
	{
		m_lodMeshes[m_currentRes]->Render();
	}
	else if (m_cModelMesh)
	{
		m_cModelMesh->Render();
	}
}

void CModel::SetColor(const float red, const float green, const float blue)
{
	this->red = red;
	this->green = green;
	this->blue = blue;
}

// Get the top left of the bounding box of the instance
Vector3 CModel::GetTopLeft(void)
{
	return Vector3(vTopLeft.x, vTopLeft.y, vTopLeft.z);
}

// Get the bottomright of the bounding box of the instance
Vector3 CModel::GetBottomRight(void)
{
	return Vector3(vBottomRight.x, vBottomRight.y, vBottomRight.z);
}

CModel::RESOLUTION_TYPE CModel::GetResolution(void) const
{
	return m_currentRes;
}

void CModel::SetResolution(RESOLUTION_TYPE type)
{
	m_currentRes = type;
}
