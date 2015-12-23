#pragma once

// STL Includes
#include <vector>

// Other Includes
#include "HighScoreRecord.h"

// Using Directives
using std::vector;

class HighScoreSystem
{
	static int s_maxRecords;
	static vector<HighScoreRecord> s_recordList;

public:
	// Call this before using it
	static void Init(int maxRecords = 10);

	// Records
	static void AddRecord(HighScoreRecord record);
	static void Reset(void);

	// Set size
	static void SetSize(int size);

	static vector<HighScoreRecord> GetRecords();
};