#pragma once

// An interface class for menus that want to process the score from the game state
class IScoreMenu
{
protected:
	int m_score;

public:
	IScoreMenu() : m_score(0)
	{

	}
	virtual ~IScoreMenu()
	{

	}

	void SetScore(int score)
	{
		m_score = score;
	}
};