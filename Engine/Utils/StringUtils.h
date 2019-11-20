#pragma once

#include <vector>
#include <string>
#include <sstream>
#include "../Core/Keyframe.h"

namespace Utils
{
	template <typename T>
	static void SubstrToVector(std::string sourceStr, int index, std::vector<T> &outVector)
	{
		std::string subStr = sourceStr.substr(index);
		std::stringstream ss(subStr);
		for (T i; ss >> i;)
		{
			outVector.push_back(i);
			if (ss.peek() == ',')
				ss.ignore();
		}
	};

	static void SubstrToFloat(std::string sourceStr, int index, float* outValue)
	{
		std::string _subStr = sourceStr.substr(index);
		*outValue = (float)std::atof(_subStr.c_str());

	}

	static void SubstrToInt(std::string sourceStr, int index, int* outValue)
	{
		std::string _subStr = sourceStr.substr(index);
		*outValue = std::atoi(_subStr.c_str());
	}

	static void SubstrToKeyframe(std::string sourceStr, Keyframe *key)
	{
		int index = (int)sourceStr.find(":");
		std::string time = sourceStr.substr(0, index);
		int valueIndex = index + 1;
		std::string value = sourceStr.substr(valueIndex);

		key->Time = (float)std::atof(time.c_str());

		std::stringstream ss(value);
		char c;
		ss >> key->Value.x;
		ss >> c;
		ss >> key->Value.y;
		ss >> c;
		ss >> key->Value.z;
		ss >> c;
		ss >> key->Value.w;
	}
}