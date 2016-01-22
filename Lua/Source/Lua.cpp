#include "Lua.h"

// STL Include
#include <cstdarg>		// For Variable Argument Lists

LuaFile::LuaFile(string luaFile)
	: m_luaState(nullptr)
{
	m_luaState = lua_open();
	luaL_openlibs(m_luaState);

	if (luaFile != "")
	{
		LoadScript(luaFile);
	}
}

LuaFile::~LuaFile(void)
{
	lua_close(m_luaState);
	m_luaState = nullptr;
}

bool LuaFile::LoadScript(string filename)
{
	if (m_luaState)
	{
		if (luaL_loadfile(m_luaState, filename.c_str()) || lua_pcall(m_luaState, 0, 0, 0))
		{
			return false;
		}

		return true;
	}

	return false;
}

LuaTypePtr LuaFile::GetValue(string varName)
{
	LuaTypePtr luaResult;

	// Only check if we have a luaState set up
	if (m_luaState)
	{
		// Push this on to the lua stack
		lua_getglobal(m_luaState, varName.c_str());

		// If this global is a number,
		if (lua_isnumber(m_luaState, lua_gettop(m_luaState)))
		{
			// Store this in a LuaNumber in luaResult
			luaResult.reset(new LuaNumber(lua_tointeger(m_luaState, lua_gettop(m_luaState))));
		}
		// If this global is a boolean,
		else if (lua_isboolean(m_luaState, lua_gettop(m_luaState)))
		{
			// Store this in a LuaBoolean in luaResult
			luaResult.reset(new LuaBoolean(lua_toboolean(m_luaState, lua_gettop(m_luaState))));
		}
		// If this global is a string,
		else if (lua_isstring(m_luaState, lua_gettop(m_luaState)))
		{
			// Store this in a LuaBoolean in luaResult
			luaResult.reset(new LuaString(lua_tostring(m_luaState, lua_gettop(m_luaState))));
		}
	}

	// Return the result
	return luaResult;
}

double LuaFile::GetNumber(string varName)
{
	return std::dynamic_pointer_cast<LuaNumber>(GetValue(varName)).get()->Number;
}

bool LuaFile::GetBoolean(string varName)
{
	return std::dynamic_pointer_cast<LuaBoolean>(GetValue(varName)).get()->Boolean;;
}

string LuaFile::GetString(string varName)
{
	return std::dynamic_pointer_cast<LuaString>(GetValue(varName)).get()->String;
}

void LuaFile::Call(string functionName, LuaTypePtr params, ...)
{
	// Get a reference to the Variable Argument List
	va_list vargList;

	// Prepare to read the Variable Argument List
	va_start(vargList, params);

	// Loop through all elements in the Variable Argument List
	for (size_t i = 0; vargList[i] != '\0'; ++i)
	{
		LuaType* lr;
		// TODO: Fill this up
	}

	// Clean up the Variable Argument List after reading
	va_end(vargList);
}
