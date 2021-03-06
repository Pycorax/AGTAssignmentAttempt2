#pragma once

#include "Grid.h"
#include "Vector3.h"
#include "..\SceneGraph\SceneNode.h"
#include "..\Occlusion\Occlusion.h"

class CSpatialPartition
{
public:
	// Constructor
	CSpatialPartition(void);
	// Destructor
	~CSpatialPartition(void);

	// Initialise the spatial partition
	bool Init(const int xSize, const int ySize, const int xNumOfGrid, const int yNumOfGrid);

	// Get xNumOfGrid
	int GetxNumOfGrid(void);
	// Get yNumOfGrid
	int GetyNumOfGrid(void);
	// Get a particular grid
	CGrid GetGrid(const int xIndex, const int yIndex);
	// Get a particular grid's num of items
	int GetGridItemSize(const int xIndex, const int yIndex) const;
	int GetGridActiveItemCount(const int xIndex, const int yIndex) const;
	// Get a grid's size
	float GetGridSizeX(void);
	float GetGridSizeY(void);
	// Get a particular grid's Mesh
	Mesh* GetGridMesh(const int xIndex, const int yIndex);

	// Get Total Min Size
	Vector3 GetMinBound(void) const;
	// Get Total Max Size
	Vector3 GetMaxBound(void) const;

	// Set a particular grid's Mesh
	void SetGridMesh(const int xIndex, const int yIndex, Mesh* theGridMesh);
	// Add a new object
	void AddObject(CSceneNode* theObject);
	// Remove a object
	void RemoveObject(CSceneNode* theObject);

	// Calculate the squared distance from camera to a grid's centrepoint
	float CalculateDistanceSquare(Vector3* theCameraPosition, const int xIndex, const int yIndex);
	// Calculate the squared distance from camera to a grid's centrepoint
	float CalculateDistanceSquare(Vector3 thePosition, Vector3 direction, const int xIndex, const int yIndex);

	// Check a position for collision with objects in any of the grids
	CSceneNode* CheckForCollision(Vector3 position);
	// Check two positions for collision with objects in any of the grids
	CSceneNode* CheckForCollision(Vector3 position_start, Vector3 position_end);

	// Update the spatial partition
	void Update(Vector3 theCameraPosition, Vector3 theCameraNormal);
	// Render the spatial partition
	void Render(Vector3* theCameraPosition = NULL);

	//PrintSelf
	void PrintSelf();

	// Variables
	CGrid* theGrid;	// Array of grids
	int xSize;		// Total horizontal size of grids
	int ySize;		// Total vertical size of grids
	int xGridSize;	// "horizontal" size of grids
	int yGridSize;	// "vertical" size of grids
	int xNumOfGrid;	// Number of horizontal grids
	int yNumOfGrid;	// Number of "vertical" grids

private:
	// Find the index of a grid which is nearest to a position
	void FindNearestGrid(Vector3 position, Vector3 direction);

	// Occlusion
	COcclusion theOcclusionChecker;
};
