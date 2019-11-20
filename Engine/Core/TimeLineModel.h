#pragma once

class TimeLineModel
{
public:
	TimeLineModel() {}
	~TimeLineModel() {}

	int CurrentFrame = 0;
	int StartFrame = 0;
	int EndFrame = 100;

	bool IsTimelineMove = false;
	bool IsPlay = false;

	int GetFrameCount()
	{
		return EndFrame - StartFrame;
	}
};