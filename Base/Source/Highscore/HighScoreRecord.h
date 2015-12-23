#pragma once

#include <string>

using std::string;

struct HighScoreRecord
{
	string levelName;
	int score;

	HighScoreRecord(string levelName_ = "", int score_ = 0) : levelName(levelName_), score(score_)
	{
	}

	void Init(string levelName_, int score_)
	{
		levelName = levelName_;
		score = score_;
	}
};