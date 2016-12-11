#pragma once

enum FrameRateRequestType
{
	FrameRateRequestType_None,
	FrameRateRequestType_TurnUp,
	FrameRateRequestType_TurnDown,
};

class FrameRateManager // Never disposed
{

public:
	FrameRateManager();

	void						Set();
	void						SetFrameTime(float CurrentFrameTime);
	bool						IsOutGrid(NiAVObject* Object);
	
	int							FrameRate;
	time_t						CurrentTime;
	float						ElapsedTime;

private:
	float						GetDistance(NiAVObject* Object);
	
	float						LastFrameTime;
	int							FrameCounter;
	int							GridDistant;
	FrameRateRequestType		RequestType;

};