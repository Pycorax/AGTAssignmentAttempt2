#pragma once

// STL Includes
#include <string>

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

class LuaFile
{
private:
	lua_State* m_luaState;

public:
	LuaFile(string luaFile = "");
	~LuaFile(void);

	// Life time
	bool LoadScript(string filename);
	void RunScript(void);

	/*
	 * Variable Acquisition
	 */
	// Functions to get values from the stack
	LuaType* GetValue(string varName);

	/*
	 * Lua Function Importing
	 */
	void Call(string functionName, LuaType* params, ...);
};