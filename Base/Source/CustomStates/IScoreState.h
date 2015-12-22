#pragma once

// An interface class that states which an IScoreScene should implement
class IScoreState
{
public:
	IScoreState()
	{

	}
	~IScoreState()
	{

	}

	virtual void SetScore(int score) = 0;
};