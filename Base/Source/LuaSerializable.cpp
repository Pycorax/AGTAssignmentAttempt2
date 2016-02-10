#include "LuaSerializable.h"

// STL Includes
#include <sstream>

// Using Directives
using std::ostringstream;

string LuaSerializable::buildPropString(string propName, string prop)
{
	ostringstream luaScript;

	luaScript << SAVE_PROPERTY_NAME << "_" << propName << " = " << prop << "\n";

	return luaScript.str();
}

string LuaSerializable::getPropString(string propName)
{
	return SAVE_PROPERTY_NAME + "_" + propName;
}
