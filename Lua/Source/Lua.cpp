#include "Lua.h"

// STL Include
#include <cstdarg>		// For Variable Argument Lists

LuaFile::LuaFile(string luaFile = "")
{
	LoadScript(luaFile);
}

LuaFile::~LuaFile(void)
{
}

void LuaFile::Call(string functionName, LuaType* params, ...)
{
	// Get a reference to the Variable Argument List
	va_list vargList;

	// Prepare to read the Variable Argument List
	va_start(vargList, params);

	// Loop through all elements in the Variable Argument List
	for (size_t i = 0; vargList[i] != '\0'; ++i)
	{
		LuaType* lr;
		//memcpy(&)
		//LuaResult lr = (LuaResult)va_arg(vargList[i], LuaResult);
	}

	// Clean up the Variable Argument List after reading
	va_end(vargList);
}
