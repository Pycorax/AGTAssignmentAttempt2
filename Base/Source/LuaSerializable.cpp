#include "LuaSerializable.h"

// STL Includes
#include <sstream>

// Using Directives
using std::ostringstream;
using std::to_string;

string LuaSerializable::BuildGenericPropString(string propName, string prop)
{
	ostringstream luaScript;

	luaScript << propName << " = " << prop << "\n";

	return luaScript.str();
}

string LuaSerializable::buildPropString(string propName, string prop, int id)
{
	ostringstream luaScript;

	luaScript << SAVE_PROPERTY_NAME;

	if (id >= 0)
	{
		// Provide the ID
		luaScript << "_" << id;
	}

	luaScript << "_" << propName << " = " << prop << "\n";

	return luaScript.str();
}

string LuaSerializable::getPropString(string propName, int id)
{
	string propString = SAVE_PROPERTY_NAME;

	if (id >= 0)
	{
		// Provide the ID
		propString += "_" + to_string(id);
	}

	propString += "_" + propName;

	return propString;
}
