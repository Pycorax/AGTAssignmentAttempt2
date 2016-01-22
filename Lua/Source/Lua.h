#pragma once

// STL Includes
#include <string>
#include <memory>

// LUA Includes
extern "C"
{
	#include "Lua/include/lua.h"
	#include "Lua/include/lualib.h"
	#include "Lua/include/lauxlib.h"
}

// Other Includes
#include "LuaType.h"

// Using Directives
using std::string;
using std::shared_ptr;

// Type Definitions
typedef shared_ptr<LuaType> LuaTypePtr;

class LuaFile
{
private:
	lua_State* m_luaState;

public:
	LuaFile(string luaFile = "");
	~LuaFile(void);

	// Life time
	bool LoadScript(string filename);
	bool RunScript(void);
	bool RunScript(string filename);

	/*
	 * Variable Acquisition
	 */
	// Functions to get values from the stack
	LuaTypePtr GetValue(string varName);
	double GetNumber(string varName);
	bool GetBoolean(string varName);
	string GetString(string varName);

	/*
	 * Lua Function Importing
	 */
	void Call(string functionName, LuaTypePtr params, ...);
};