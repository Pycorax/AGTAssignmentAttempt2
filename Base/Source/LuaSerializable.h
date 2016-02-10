/******************************************************************************/
/*!
\file	LuaSerializable.h
\author Tng Kah Wei
\brief	Interface class that allows derived classes to implement save/load
		via Lua files
*/
/******************************************************************************/
#pragma once

// STL Includes
#include <string>

// API Includes
#include "Lua.h"

// Using Directives
using std::string;
using Lua::LuaFile;

/******************************************************************************/
/*!
		Class LuaSerializable:
\brief	Interface class that allows derived classes to implement save/load
		via Lua files.

		To use, override the abstract functions defined below.
*/
/******************************************************************************/
class LuaSerializable
{


protected:
	const string SAVE_PROPERTY_NAME;

public:
	LuaSerializable(string savePropertyName) : SAVE_PROPERTY_NAME(savePropertyName) {}
	virtual ~LuaSerializable() {}

	static string BuildGenericPropString(string propName, string prop);

	/*
	 * Implement abstract functions
	 */
	virtual string SaveStatus(int id = -1) = 0;
	virtual void LoadStatus(LuaFile* L, int id = -1) = 0;

protected:
	// Use an ID if dealing with multiple similar objects to identify different objects
	string buildPropString(string propName, string prop, int id = -1);			
	string getPropString(string propName, int id = -1);
};