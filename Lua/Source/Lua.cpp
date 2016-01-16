#include "Lua.h"

// STL Include
#include <cstdarg>		// For Variable Argument Lists

Lua::Lua(void)
{
}

Lua::~Lua(void)
{
}

void Lua::Call(string functionName, LuaResult params, ...)
{
	// Get a reference to the Variable Argument List
	va_list vargList;

	// Prepare to read the Variable Argument List
	va_start(vargList, params);

	// Loop through all elements in the Variable Argument List
	for (size_t i = 0; vargList[i] != '\0'; ++i)
	{
		LuaResult lr;
		//memcpy(&)
		//LuaResult lr = (LuaResult)va_arg(vargList[i], LuaResult);
	}

	// Clean up the Variable Argument List after reading
	va_end(vargList);
}
