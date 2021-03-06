#pragma once
#include "node.h"
#include "Model.h"
#include "Transform.h"
//#include <list>

#include <vector>
using namespace std;

class CSceneManager;
class CSpatialPartition;

class CSceneNode : 	public CNode
{
public:
	enum NODE_TYPE
	{
		NT_NORMAL,
		NT_BOMBER,
		NT_TOTAL
	};

protected:
	// Children Nodes
	vector<CSceneNode*> theChildren;

	// Mesh
	CModel* theModel;

	// Transform
	CTransform* theTransform;

	// IDs for locating spatial details of this node
	int sceneNodeID;
	int gridID;
	int secondaryGridID;
	CSpatialPartition* spatialPartition;		// Pointer to the spatial partition system that houses this object

	// If this node is active
	bool active;

	// The type of node
	NODE_TYPE type;

	

public:
	CSceneNode(void);
	CSceneNode(const int sceneNodeID);
	~CSceneNode(void);

	void Update(CModel::RESOLUTION_TYPE resType);

	// Draw this Node and its children
	void Draw(void);
	void Draw(CSceneManager* theSceneManager);

	// Set model and other info to this Node
	void SetModel(CModel* theModel);
	int SetNode(CTransform* aNewTransform, CModel* aNewModel);
	void SetSceneNodeID(const int sceneNodeID);

	int AddChild(CTransform* aNewTransform, CModel* aNewModel);
	int AddChild(const int sceneNodeID, CTransform* aNewTransform, CModel* aNewModel);
	int AddChild(CSceneNode* newNode);

	// Get methods
	int GetSceneNodeID(void);
	CSceneNode* GetNode(const int sceneNodeID);
	vector<CSceneNode*> GetChildren(void) const;

	// Get Transforms
	Vector3 GetTranslate(void) const;
	Vector3 GetScale(void) const;

	// Apply transformation to this Node and its children
	void ApplyTranslate( const float dx, const float dy, const float dz );
	void ApplyRotate( const float angle, const float rx, const float ry, const float rz );
	void ApplyScale(const float sx, const float sy, const float sz);

	// Direct Sets of Transformations
	void SetTranslate(const float dx, const float dy, const float dz);
	void ResetTransform(void);

	// Get top left corner of the group
	Vector3 GetTopLeft(void);
	// Get bottom right corner of the group
	Vector3 GetBottomRight(void);

	// Set colour of the instance
	void SetColor(const float red, const float green, const float blue);

	// Return the number of children in this group
	int GetNumOfChild(void);
	// Get top left corner of a child
	bool GetTopLeft(const int m_iChildIndex, Vector3& vector3D_TopLeft);
	// Get bottom right corner of a child
	bool GetBottomRight(const int m_iChildIndex, Vector3& vector3D_BottomRight);

	// Set the Colour of a child
	void SetColorForChild(const int m_iChildIndex, const float red, const float green, const float blue);

	// Check a position for collision with objects in any of the grids
	CSceneNode* CheckForCollision(Vector3 position);
	// Check two positions for collision with objects in any of the grids
	CSceneNode* CheckForCollision(Vector3 position_start, Vector3 position_end, Vector3 &Hit);

	// For Spatial Partitioning
	void SetGridID(int gridID);
	void SetSecondaryGridID(int gridID);
	int GetGridID(void);
	int GetSecondaryGridID(void);
	void SetSpatialPartition(CSpatialPartition* partition);

	// Active
	bool GetActive();
	void Activate();
	void Deactivate();

	// Type
	NODE_TYPE GetType() const;
	void SetType(NODE_TYPE type);

private:
	int getIntersection(float fDst1, float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit);
	int inBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis);
};
