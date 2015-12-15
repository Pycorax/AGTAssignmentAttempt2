#include "Transform.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#include <windows.h> // Header File For Windows
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library

CTransform::CTransform(void)
{
	Mtx.SetToZero();
	Mtx.SetToIdentity();
	Update_Mtx.SetToZero();
	Update_Mtx.SetToIdentity();
}

CTransform::CTransform( const float dx, const float dy, const float dz )
{
	Mtx.SetToTranslation( dx, dy, dz );
}

CTransform::~CTransform(void)
{
}

void CTransform::PreRendering()
{
	glPushMatrix();
	glMultMatrixf(Mtx.a);
}

void CTransform::Draw(void)
{
}

void CTransform::PostRendering()
{
	glPopMatrix();
}

void CTransform::SetTranslate( const float dx, const float dy, const float dz  )
{
	Mtx44 TempMtx;
	TempMtx.SetToTranslation( dx, dy, dz );

	Mtx = Mtx * TempMtx;
}

void CTransform::SetRotate( const float angle, const float rx, const float ry, const float rz  )
{
	Mtx44 TempMtx;
	TempMtx.SetToRotation( angle, rx, ry, rz );
	
	Mtx44 MtxBackToOrigin;

	MtxBackToOrigin.a[ 12 ] = -Mtx.a[ 12 ];
	MtxBackToOrigin.a[ 13 ] = -Mtx.a[ 13 ];
	MtxBackToOrigin.a[ 14 ] = -Mtx.a[ 14 ];

	Mtx44 MtxBackToPosition;
	MtxBackToPosition.a[ 12 ] = Mtx.a[ 12 ];
	MtxBackToPosition.a[ 13 ] = Mtx.a[ 13 ];
	MtxBackToPosition.a[ 14 ] = Mtx.a[ 14 ];

	Mtx = Mtx * MtxBackToOrigin;
	Mtx = Mtx * TempMtx;
	Mtx = Mtx * MtxBackToPosition;
}

void CTransform::SetScale( const float sx, const float sy, const float sz  )
{
	Mtx.SetToScale( sx, sy, sz );
}

void CTransform::GetOffset( float& x, float& y, float& z )
{
	x = Mtx.a[ 12 ];
	y = Mtx.a[ 13 ];
	z = Mtx.a[ 14 ];
}

// Get the transformation matrix
Mtx44 CTransform::GetTransform(void)
{
	return Mtx;
}