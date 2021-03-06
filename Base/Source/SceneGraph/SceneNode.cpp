#include "SceneNode.h"
#include "../SceneManager.h"
#include "Vector3.h"

#include <cstdlib>

CSceneNode::CSceneNode(void)
: theModel( NULL )
, theChildren( NULL )
, theTransform ( NULL )
, sceneNodeID (-1)
, gridID(-1)
, secondaryGridID(-1)
, spatialPartition(nullptr)
, active(true)
, type(NT_NORMAL)
{
}

CSceneNode::CSceneNode(const int sceneNodeID)
{
	CSceneNode();
	SetSceneNodeID( sceneNodeID );
}


CSceneNode::~CSceneNode(void)
{
	if (theChildren.size() != 0)
	{
		CNode* aChild = NULL;
		for (unsigned i=0; i<theChildren.size(); i++)
		{
			aChild = theChildren[i];
			delete aChild;
			aChild = NULL;
		}
		theChildren.clear();
	}

	if (theModel != NULL)
	{
		delete theModel;
		theModel = NULL;
	}

	if (theTransform != NULL)
	{
		delete theTransform;
		theTransform = NULL;
	}
}

void CSceneNode::Update(CModel::RESOLUTION_TYPE resType)
{
	// Update the current model
	if (theModel)
	{
		theModel->SetResolution(resType);
	}

	// Update children as well
	for (auto child : theChildren)
	{
		child->Update(resType);
	}
}

void CSceneNode::Draw(void)
{
	if (!active)
	{
		return;
	}

	if (theTransform)
	{
		theTransform->PreRendering();
		if (theModel)
		{
			theModel->Draw();
		}

		if (theChildren.size() != 0)
		{
			CNode* aChild = NULL;
			for (unsigned i=0; i<theChildren.size(); i++)
			{
				aChild = theChildren[i];
				aChild->Draw();
			}
		}

		theTransform->PostRendering();
	}
}

void CSceneNode::Draw(CSceneManager* theSceneManager)
{
	if (!active)
	{
		return;
	}

	if (theTransform)
	{
		theSceneManager->PreRendering(theTransform->GetTransform(), false);

		if (theModel)
		{
			theModel->Draw();
		}

		if (theChildren.size() != 0)
		{
			CNode* aChild = NULL;
			for (unsigned i=0; i<theChildren.size(); i++)
			{
				aChild = theChildren[i];
				((CSceneNode*)aChild)->Draw(theSceneManager);
			}
		}

		theSceneManager->PostRendering();
	}
}

void CSceneNode::SetModel(CModel* theModel)
{
	this->theModel = theModel;
}

int CSceneNode::SetNode(CTransform* aNewTransform, CModel* aNewModel)
{
	this->theModel = aNewModel;
	this->theTransform = aNewTransform;

	// Set sceneNodeID to 1 as this is the root node
	sceneNodeID = 1;
	return sceneNodeID;
}

void CSceneNode::SetSceneNodeID(const int sceneNodeID)
{
	this->sceneNodeID = sceneNodeID;
}

int CSceneNode::AddChild(CTransform* aNewTransform, CModel* aNewModel)
{
	CSceneNode* aNewNode = new CSceneNode();
	aNewNode->SetNode( aNewTransform, aNewModel );
	aNewNode->SetSceneNodeID( sceneNodeID*100 + (theChildren.size()+1) );
	theChildren.push_back( aNewNode );

	return aNewNode->GetSceneNodeID();
}

int CSceneNode::AddChild(const int sceneNodeID, CTransform* aNewTransform, CModel* aNewModel)
{
	CSceneNode* aNewNode = new CSceneNode( sceneNodeID );
	aNewNode->SetNode( aNewTransform, aNewModel );
	theChildren.push_back( aNewNode );

	return aNewNode->GetSceneNodeID();
}

int CSceneNode::AddChild(CSceneNode * newNode)
{
	newNode->SetSceneNodeID(sceneNodeID * 100 + (theChildren.size() + 1));
	theChildren.push_back(newNode);

	return newNode->GetSceneNodeID();
}

int CSceneNode::GetSceneNodeID(void)
{
	return sceneNodeID;
}

CSceneNode* CSceneNode::GetNode(const int sceneNodeID)
{
	if (this->sceneNodeID == sceneNodeID)
		return this;

	CSceneNode* theTarget = NULL;
	if (theChildren.size() != 0)
	{
		CSceneNode* aChild = NULL;
		for (unsigned i=0; i<theChildren.size(); i++)
		{
			aChild = (CSceneNode*)theChildren[i];

			theTarget = ((CSceneNode*)aChild)->GetNode( sceneNodeID );
			if (theTarget != NULL)
				break;
		}
	}

	return theTarget;
}

vector<CSceneNode*> CSceneNode::GetChildren(void) const
{
	return theChildren;
}

Vector3 CSceneNode::GetTranslate(void) const
{
	Mtx44 tf = theTransform->GetTransform();

	return Vector3(tf.a[12], tf.a[13], tf.a[14]);
}

Vector3 CSceneNode::GetScale(void) const
{
	Mtx44 tf = theTransform->GetTransform();

	return Vector3(tf.a[0], tf.a[5], tf.a[10]);
}

void CSceneNode::ApplyTranslate( const float dx, const float dy, const float dz )
{
	if (theTransform)
	{
		// Remove it first
		if (spatialPartition)
		{
			spatialPartition->RemoveObject(this);
		}

		theTransform->SetTranslate(dx, dy, dz);

		// Update the Grid positioning after u change position
		if (spatialPartition)
		{
			spatialPartition->AddObject(this);
		}
	}	
}


void CSceneNode::ApplyRotate( const float angle, const float rx, const float ry, const float rz )
{
	if (theTransform)
	{
		// Then rotate the children
		theTransform->SetRotate( angle, rx, ry, rz );
	}
}

void CSceneNode::ApplyScale(const float sx, const float sy, const float sz)
{
	if (theTransform)
	{
		theTransform->SetScale(sx, sy, sz);
	}
}

// Note: DO NOT USE WITH ROTATION
void CSceneNode::SetTranslate(const float dx, const float dy, const float dz)
{
	spatialPartition->RemoveObject(this);

	if (theTransform)
	{
		// Store the scale
		Vector3 scale(theTransform->GetTransform().a[0], theTransform->GetTransform().a[5], theTransform->GetTransform().a[10]);

		// Reset the Mtx44 to discard the translation
		theTransform->Reset();

		// Apply the translation
		theTransform->SetTranslate(dx, dy, dz);

		// Reapply the Scale
		theTransform->SetScale(scale.x, scale.y, scale.z);
	}

	spatialPartition->AddObject(this);
}

void CSceneNode::ResetTransform(void)
{
	if (theTransform)
	{
		theTransform->Reset();
	}
}


// Get top left corner of the group
Vector3 CSceneNode::GetTopLeft(void)
{
	if (theTransform == NULL)
		return Vector3( theModel->GetTopLeft().x, 
						 theModel->GetTopLeft().y, 
						 theModel->GetTopLeft().z);
	else
	{
		return (theTransform->GetTransform() * theModel->GetTopLeft());
	}
}

// Get bottom right corner of the group
Vector3 CSceneNode::GetBottomRight(void)
{
	if (theTransform == NULL)
		return Vector3( theModel->GetBottomRight().x, 
						 theModel->GetBottomRight().y, 
						 theModel->GetBottomRight().z);
	else
		return (theTransform->GetTransform() * theModel->GetBottomRight());
}

void CSceneNode::SetColor(const float red, const float green, const float blue)
{
	theModel->SetColor(red, green, blue);
}

// Return the number of children in this group
int CSceneNode::GetNumOfChild(void)
{
	return (int)theChildren.size();
}

// Get top left corner of a child
bool CSceneNode::GetTopLeft(const int m_iChildIndex, Vector3& Vector3_TopLeft)
{
	Vector3_TopLeft = Vector3( -999, -999, -999 );
	CSceneNode* aChild = NULL;
	for (unsigned i = 0; i < theChildren.size(); i++)
	{
		aChild = (CSceneNode*)theChildren[i];

		if (aChild->GetSceneNodeID() == m_iChildIndex)
		{
			Vector3_TopLeft = theTransform->GetTransform() * aChild->GetTopLeft();
			return true;
		}
	}
	return false;
}

// Get bottom right corner of a child
bool CSceneNode::GetBottomRight(const int m_iChildIndex, Vector3& Vector3_BottomRight)
{
	Vector3_BottomRight = Vector3( -999, -999, -999 );
	CSceneNode* aChild = NULL;
	for (unsigned i=0; i<theChildren.size(); i++)
	{
		aChild = (CSceneNode*)theChildren[i];

		if (aChild->GetSceneNodeID() == m_iChildIndex)
		{
			Vector3_BottomRight = theTransform->GetTransform() * aChild->GetBottomRight();
			return true;
		}
	}
	return false;
}

void CSceneNode::SetColorForChild(const int m_iChildIndex, const float red, const float green, const float blue)
{
	CSceneNode* aChild = NULL;
	for (unsigned i=0; i<theChildren.size(); i++)
	{
		aChild = (CSceneNode*)theChildren[i];

		if (aChild->GetSceneNodeID() == m_iChildIndex)
		{
			aChild->SetColor(red, green, blue);
		}
	}
}

/********************************************************************************
 Check a position for collision with objects in any of the grids
 ********************************************************************************/
CSceneNode* CSceneNode::CheckForCollision(Vector3 position)
{
	if (!active)
	{
		return nullptr;
	}

	for (auto node = theChildren.begin(); node != theChildren.end(); ++node)
	{
		CSceneNode* sNode = dynamic_cast<CSceneNode*>(*node);

		if (sNode)
		{
			// Calculate the relative position of the "position"
			Vector3 relativePos = theTransform->GetTransform().GetInverse() * position;

			if (CSceneNode* node = sNode->CheckForCollision(relativePos))
			{
				return node;
			}
		}
	}

	Vector3 ObjectTopLeft     = GetTopLeft();
	Vector3 ObjectBottomRight = GetBottomRight();

	if ( ((ObjectTopLeft.x > position.x) && (ObjectTopLeft.y > position.y) && (ObjectTopLeft.z > position.z)) &&
			((ObjectBottomRight.x < position.x) && (ObjectBottomRight.y < position.y) && (ObjectBottomRight.z < position.z)) )
	{
		return this;
	}
	return nullptr;
}

/********************************************************************************
Check a position for collision with objects in any of the grids
********************************************************************************/
CSceneNode* CSceneNode::CheckForCollision(Vector3 position_start, Vector3 position_end, Vector3 &Hit)
{
	if (!active)
	{
		return false;
	}

	for (auto node = theChildren.begin(); node != theChildren.end(); ++node)
	{
		CSceneNode* sNode = dynamic_cast<CSceneNode*>(*node);

		if (sNode)
		{
			// Calculate the relative position of the "position"
			Mtx44 inverseTransform = theTransform->GetTransform().GetInverse();
			Vector3 relativePosStart = inverseTransform * position_start;
			Vector3 relativePosEnd = inverseTransform * position_end;

			if (CSceneNode* node = sNode->CheckForCollision(relativePosStart, relativePosEnd, Hit))
			{
				return this;
			}
		}
	}

	Vector3 ObjectTopLeft = GetTopLeft();
	Vector3 ObjectBottomRight = GetBottomRight();

	if (position_end.x < ObjectBottomRight.x && position_start.x < ObjectBottomRight.x)
	{
		return nullptr;
	}

	if (position_end.x > ObjectTopLeft.x && position_start.x > ObjectTopLeft.x)
	{
		return nullptr;
	}

	if (position_end.y < ObjectBottomRight.y && position_start.y < ObjectBottomRight.y)
	{
		return nullptr;
	}

	if (position_end.y > ObjectTopLeft.y && position_start.y > ObjectTopLeft.y)
	{
		return nullptr;
	}

	if (position_end.x < ObjectBottomRight.x && position_start.x < ObjectBottomRight.x)
	{
		return nullptr;
	}

	if (position_end.x > ObjectTopLeft.x && position_start.x > ObjectTopLeft.x)
	{
		return nullptr;
	}

	if (position_start.x > ObjectBottomRight.x && position_start.x < ObjectTopLeft.x &&
		position_start.y > ObjectBottomRight.y && position_start.y < ObjectTopLeft.y &&
		position_start.z > ObjectBottomRight.z && position_start.z < ObjectTopLeft.z)
	{
		Hit = position_start;
		return this;
	}

	if (
		(
			getIntersection(position_start.x - ObjectBottomRight.x, position_end.x - ObjectBottomRight.x, position_start, position_end, Hit) 
			&&
			inBox(Hit, ObjectBottomRight, ObjectTopLeft, 1)
		) 
		||
		(
			getIntersection(position_start.y - ObjectBottomRight.y, position_end.y - ObjectBottomRight.y, position_start, position_end, Hit) 
			&&
			inBox(Hit, ObjectBottomRight, ObjectTopLeft, 2)
		)
		||
		(
			getIntersection(position_start.z - ObjectBottomRight.z, position_end.z - ObjectBottomRight.z, position_start, position_end, Hit) 
			&&
			inBox(Hit, ObjectBottomRight, ObjectTopLeft, 3))
		|| 
		(
			getIntersection(position_start.x - ObjectTopLeft.x, position_end.x - ObjectTopLeft.x, position_start, position_end, Hit) 
			&&
			inBox(Hit, ObjectBottomRight, ObjectTopLeft, 1)
		) 
		||
		(
			getIntersection(position_start.y - ObjectTopLeft.y, position_end.y - ObjectTopLeft.y, position_start, position_end, Hit) 
			&&
			inBox(Hit, ObjectBottomRight, ObjectTopLeft, 2)
		)
		||
		(
			getIntersection(position_start.z - ObjectTopLeft.z, position_end.z - ObjectTopLeft.z, position_start, position_end, Hit) 
			&&
			inBox(Hit, ObjectBottomRight, ObjectTopLeft, 3)
			)
		)
	{
		return this;
	}

	return nullptr;
}

void CSceneNode::SetGridID(int gridID)
{
	this->gridID = gridID;
}

void CSceneNode::SetSecondaryGridID(int gridID)
{
	secondaryGridID = gridID;
}

int CSceneNode::GetGridID(void)
{
	return gridID;
}

int CSceneNode::GetSecondaryGridID(void)
{
	return secondaryGridID;
}

void CSceneNode::SetSpatialPartition(CSpatialPartition * partition)
{
	spatialPartition = partition;
}

bool CSceneNode::GetActive()
{
	return active;
}

void CSceneNode::Activate()
{
	// Do for all children
	for (auto node = theChildren.begin(); node != theChildren.end(); ++node)
	{
		CSceneNode* sNode = dynamic_cast<CSceneNode*>(*node);

		if (sNode)
		{
			sNode->Activate();
		}

	}

	// Set active to true
	active = true;
	// Add back to the List
	spatialPartition->AddObject(this);
}

void CSceneNode::Deactivate()
{
	// Do for all children
	for (auto node = theChildren.begin(); node != theChildren.end(); ++node)
	{
		CSceneNode* sNode = dynamic_cast<CSceneNode*>(*node);

		if (sNode)
		{
			sNode->Deactivate();
		}

	}

	// Set active to false
	active = false;

	// Remove from the List
	spatialPartition->RemoveObject(this);
}

CSceneNode::NODE_TYPE CSceneNode::GetType() const
{
	return type;
}

void CSceneNode::SetType(NODE_TYPE type)
{
	this->type = type;
}

int CSceneNode::getIntersection(float fDst1, float fDst2, Vector3 P1, Vector3 P2, Vector3 & Hit)
{
	if ((fDst1 * fDst2) >= 0.0f)
	{
		return 0;
	}

	if (fDst1 == fDst2)
	{
		return 0;
	}

	Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));

	return 1;
}

int CSceneNode::inBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis)
{
	if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y)
	{
		return 1;
	}
	if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x)
	{
		return 1;
	}
	if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y)
	{
		return 1;
	}
	return 0;
}
