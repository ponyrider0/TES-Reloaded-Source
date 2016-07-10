#pragma once

enum RequestEnum
{
	None,
	TurnUp,
	TurnDown,
};

class FrameRateManager // Never disposed
{

public:
	FrameRateManager();

	void						SetFrame(int FrameRate);
	bool						IsOutGrid(NiAVObject* Object);
	
private:
	void						SetNearObjects();
	float						GetDistance(NiAVObject* Object);
	int							GridDistant;
	int							FrameCounter;
	RequestEnum					Request;

};