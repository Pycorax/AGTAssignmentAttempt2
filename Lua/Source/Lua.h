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

// Using Directives
using std::string;

class LuaFile
{
private:
	lua_State* m_luaState;

public:
	union LuaResult
	{
		int* Integer;
		double* Number;
		bool* Boolean;
		string* String;
	};

	LuaFile(string luaFile = "");
	~LuaFile(void);

	// Life time
	bool LoadScript(string filename);
	void RunScript(void);

	/*
	 * Variable Acquisition
	 */
	// Functions to get values from the stack
	LuaResult GetValue(string varName);

	/*
	 * Lua Function Importing
	 */
	void Call(string functionName, LuaResult params, ...);
};