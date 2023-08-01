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

	static glm::vec4 RandomOnSphere(float r)
	{
		float pi = 3.14159265359;
		// Take two random angles
		float theta = Random::RandomFloat(0.0f, 2.0f * pi);
		float phi = Random::RandomFloat(0.0f, pi);
		float x = r * sin(phi) * cos(theta);
		float y = r * sin(phi) * sin(theta);
		float z = r * cos(phi);

		return glm::vec4(x, y, z, 1.0f);
	}

	static float RandomSpread(float center, float spread)
	{
		return RandomFloat(center - spread, center + spread);
	}

	static glm::vec2 RandomSpreadVec2(const glm::vec2& center, const glm::vec2& spread)
	{
		return glm::vec2(RandomSpread(center.x, spread.x), RandomSpread(center.y, spread.y));
	}

	static glm::vec2 RandomUnitVec2OnCircle(float center, float spread)
	{
		float angle = RandomSpread(center, spread);
		return UnitVec2OnCircle(angle);
	}
private:
	// Helper Functions
	static glm::vec2 UnitVec2OnCircle(float angle)
	{
		return glm::vec2(cos(angle), sin(angle));
	}

};