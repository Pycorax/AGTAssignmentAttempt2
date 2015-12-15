#include "Goodies.h"

CGoodies::CGoodies(void)
: pos_x(0)
, pos_y(0)
, theMesh(NULL)
{
}

CGoodies::~CGoodies(void)
{
	if (theMesh != NULL)
	{
		delete theMesh;
		theMesh = NULL;
	}
}

// Get the pos_x of this goodies
int CGoodies::GetPos_x(void)
{
	return pos_x;
}

// Get the pos_y of this goodies
int CGoodies::GetPos_y(void)
{
	return pos_y;
}

// Set the pos_x and pos_y of this goodies
void CGoodies::SetPos(const int pos_x, const int pos_y)
{
	this->pos_x = pos_x;
	this->pos_y = pos_y;
}

// Get the Mesh
Mesh* CGoodies::GetMesh(void)
{
	return theMesh;
}

// Set the Mesh
void CGoodies::SetMesh(Mesh* theMesh)
{
	this->theMesh = theMesh;
}

// Set the Texture ID
void CGoodies::SetTextureID(const int TextureID)
{
	if ((this->theMesh != NULL) && (TextureID != 0))
	{
		this->theMesh->textureID = TextureID;
	}
}
