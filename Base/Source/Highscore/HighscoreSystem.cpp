#include "HighscoreSystem.h"

// Default records is 10
int HighScoreSystem::s_maxRecords = 10;
vector<HighScoreRecord> HighScoreSystem::s_recordList;

void HighScoreSystem::Init(int maxRecords)
{
	s_maxRecords = maxRecords;
	Reset();
}

void HighScoreSystem::AddRecord(HighScoreRecord record)
{
	// Tracker to check if we should actually add it in
	bool eligible = false;

	if (s_recordList.size() < s_maxRecords)
	{
		eligible = true;
	}
	else
	{
		// Check if this score beats any of the current record holders
		for (auto r = s_recordList.begin(); r != s_recordList.end(); ++r)
		{
			if (record.score > r->score)
			{
				eligible = true;
				break;
			}
		}
	}

	// If ineligible, screw it, don't bother anymore
	if (eligible)
	{
		s_recordList.push_back(record);

		// Sort the list
		// -- Copy the list of items
		vector<HighScoreRecord> records = s_recordList;
		// -- Clear the record list
		Reset();
		// -- Copy over in the correct order
		for (int i = 0; i < s_maxRecords && !records.empty(); ++i)		// For every record
		{
			auto highestIter = records.begin();
			// Check what is the highest
			for (auto r = records.begin(); r != records.end(); ++r)
			{
				if (r->score > highestIter->score)
				{
					highestIter = r;
				}
			}
			// Add the highest to the front of the highscore list
			s_recordList.push_back(*highestIter);
			// Remove this record from the searching list
			records.erase(highestIter);
		}
	}
}

void HighScoreSystem::Reset(void)
{
	s_recordList.clear();
}

void HighScoreSystem::SetSize(int size)
{
	s_maxRecords = size;
}

vector<HighScoreRecord> HighScoreSystem::GetRecords()
{
	return s_recordList;
}
