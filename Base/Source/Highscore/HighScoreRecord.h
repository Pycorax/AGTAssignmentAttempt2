#pragma once

#include <string>

using std::string;

struct HighScoreRecord
{
	string levelName;
	int score;

	HighScoreRecord() : levelName(""), score(0)
	{
	}

	void Init(string levelName_, int score_)
	{
		levelName = levelName_;
		score = score_;
	}
};