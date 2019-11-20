#include "Animation.h"

Animation::Animation()
{
}


Animation::~Animation()
{
}

bool Animation::HasKey()
{
	bool res = false;
	if (TranslateKeys.size() > 0)
	{
		res = true;
	}
	if (ScaleKeys.size() > 0)
	{
		res = true;
	}
	if (RotateKeys.size() > 0)
	{
		res = true;
	}
	if (ColorKeys.size() > 0)
	{
		res = true;
	}
	return res;
}


void Animation::SetTranslateKey(float time, glm::vec4 translate)
{
	Keyframe newKey = FindInKey(time, TranslateKeys);

	if (newKey.Index < 0)
	{
		InsertKey(time, translate, TranslateKeys);
	}
	else
	{
		TranslateKeys[newKey.Index].Value = translate;
	}
}

void Animation::SetRotateKey(float time, glm::vec4 rotate)
{
	Keyframe newKey = FindInKey(time, RotateKeys);
	if (newKey.Index < 0)
	{
		InsertKey(time, rotate, RotateKeys);
	}
	else
	{
		RotateKeys[newKey.Index].Value = rotate;
	}
}

void Animation::SetScaleKey(float time, glm::vec4 scale)
{
	Keyframe newKey = FindInKey(time, ScaleKeys);
	if (newKey.Index < 0)
	{
		InsertKey(time, scale, ScaleKeys);
	}
	else
	{
		ScaleKeys[newKey.Index].Value = scale;
	}
}

void Animation::SetColorKey(float time, glm::vec4 color)
{
	Keyframe newKey = FindInKey(time, ColorKeys);
	if (newKey.Index < 0)
	{
		InsertKey(time, color, ColorKeys);
	}
	else
	{
		ColorKeys[newKey.Index].Value = color;
	}
}

void Animation::InsertKey(float time, glm::vec4 value, std::vector<Keyframe> &Keys)
{
	if (Keys.size() == 0)
	{
		Keys.push_back(Keyframe(0, time, value));
	}
	else 
	{
		float iterTime = time;
		Keyframe key;
		while (key.Index < 0 && iterTime <= Keys[Keys.size() - 1].Time)
		{
			key = FindInKey(iterTime, Keys);
			iterTime++;
		}

		int index;
		if (key.Index > 0)
		{
			index = key.Index;

			// Shift Keys from insert position to right each one frame
			for (int i = key.Index; i < Keys.size(); i++)
			{
				Keys[i].Index = i + 1;
			}
		}
		else
		{
			if (key.Index == 0)
			{
				index = 0;

				// Shift All Keys to right each one frame
				for (int i = 0; i < Keys.size(); i++)
				{
					Keys[i].Index = i + 1;
				}
			}
			else
			{
				index = (int)Keys.size();
			}
		}
		Keys.insert(Keys.begin() + index, Keyframe(index, time, value));
	}
}

Keyframe Animation::FindInKey(const float time, std::vector<Keyframe> &Keys)
{
	Keyframe result;
	
	std::vector<Keyframe>::iterator it = std::find_if(Keys.begin(), Keys.end(), MatchTime(time));
	
	if (it != Keys.end())
	{
		result = *it;
	}

	return result;
}

glm::vec4 Animation::GetValueOnTime(const float time, std::vector<Keyframe> &Keys)
{
	glm::vec4 outValue;
	int iterTime = (int)time;
	Keyframe secondKey;

	while (secondKey.Index < 0 && iterTime <= Keys[Keys.size() - 1].Time)
	{
		secondKey = FindInKey((float)iterTime, Keys);
		iterTime++;
	}

	if (secondKey.Index > 0)
	{
		Keyframe firstKey = Keys[secondKey.Index - 1];

		float clampedTime = glm::clamp(time, firstKey.Time, secondKey.Time);

		outValue = glm::mix(firstKey.Value, secondKey.Value,
			(clampedTime - firstKey.Time) / (secondKey.Time - firstKey.Time));
	}
	else
	{
		if (secondKey.Index == 0)
		{
			outValue = Keys[0].Value;
		}
		else
		{
			outValue = Keys[Keys.size() - 1].Value;
		}
	}

	return outValue;
}

float Animation::GetEndTime()
{
	float endTime1 = 0.0f;
	if (TranslateKeys.size() > 0)
	{
		endTime1 = TranslateKeys[TranslateKeys.size() - 1].Time;
	}

	float endTime2 = 0.0f;
	if (ScaleKeys.size() > 0)
	{
		endTime2 = ScaleKeys[ScaleKeys.size() - 1].Time;
	}

	float endTime3 = 0.0f;
	if (RotateKeys.size() > 0)
	{
		endTime3 = RotateKeys[RotateKeys.size() - 1].Time;
	}

	float endTime4 = 0.0f;
	if (ColorKeys.size() > 0)
	{
		endTime4 = ColorKeys[ColorKeys.size() - 1].Time;
	}

	return std::max(std::max(endTime1, endTime2), std::max(endTime3, endTime4));
}

//float Animation::GetStartTime()
//{
//	float startTime1 = GetEndTime();
//	if (object->animation.TranslateKeys.size() > 0)
//	{
//		startTime1 = object->animation.TranslateKeys[0].Time;
//	}
//
//	float startTime2 = GetEndTime();
//	if (object->animation.ScaleKeys.size() > 0)
//	{
//		startTime2 = object->animation.ScaleKeys[0].Time;
//	}
//
//	float startTime3 = GetEndTime();
//	if (object->animation.RotateKeys.size() > 0)
//	{
//		startTime3 = object->animation.RotateKeys[0].Time;
//	}
//
//	return std::min(std::min(startTime1, startTime2), startTime3);
//}