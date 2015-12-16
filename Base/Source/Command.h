#pragma once

// STL Includes
#include <string>

// Using Directives
using std::string;

// Template for a Command data structure following the Command Data Structure. Command has a tag which can be  used for simple Command techniques.
class Command
{
protected:
	string m_tag;

public:
	Command(std::string tag)
		: m_tag(tag)
	{
	
	}

	virtual ~Command()
	{
		
	}

	string GetTag()
	{
		return m_tag;
	}
};