#pragma once


#include <random>

/*
	Singleton Random Class
*/
class Random
{
public:
	static float RandomFloat(float min, float max)
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<float> dist(min, max);
		return dist(mt);
	}
};