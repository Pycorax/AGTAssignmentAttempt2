#pragma once

// STL Includes
#include <string>

// Using Directives
using std::string;

struct LuaType
{
	enum LUA_TYPE
	{
		LT_NUMBER,
		LT_BOOL,
		LT_STRING,
		LT_TOTAL
	};

	virtual ~LuaType() {}
};

struct LuaNumber : public LuaType
{
	double Number;

	LuaNumber(double number = 0) : Number(number) {}
	~LuaNumber() {}
};

struct LuaBoolean : public LuaType
{
	bool Boolean;

	LuaBoolean(bool boolean = 0) : Boolean(boolean) {}
	~LuaBoolean() {}
};

struct LuaString : public LuaType
{
	string String;

	LuaString(string str = 0) : String(str) {}
	~LuaString() {}
};