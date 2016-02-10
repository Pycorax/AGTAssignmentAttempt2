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

	/*
	 * Implement abstract functions
	 */
	virtual string SaveStatus(void) = 0;
	virtual void LoadStatus(LuaFile* L) = 0;

protected:
	string buildPropString(string propName, string prop);
	string getPropString(string propName);
};