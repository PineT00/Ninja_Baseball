#include "pch.h"
#include "Random.h"
#include <random>

namespace Utils
{
	float Random::RandomValue()
	{
		static std::random_device rd;

		static std::mt19937 gen(rd());

		std::uniform_real_distribution<float> dis(0.0f, 1.0f);

		return dis(gen);
	}

	int Random::RandomRange(int min, int max)
	{
		static std::random_device rd;

		static std::mt19937 generator(rd());

		std::uniform_int_distribution<int> distribution(min, max);

		return distribution(generator);
	}

	float Random::RandomRange(float min, float max)
	{
		float value = RandomValue();
		value *= (max - min);
		value += min;

		return value;
	}

	sf::Vector2f Random::RandomOnUnitCircle()
	{
		sf::Transform rotation;
		rotation.rotate(RandomRange(0, 360));
		return rotation * sf::Vector2f{ 1.f, 0.f };
	}

	sf::Vector2f Random::RandomInUnitCircle()
	{
		return RandomOnUnitCircle() * RandomValue();
	}

	sf::Vector2f Random::GetRandomVector2(float start, float end)
	{
		return sf::Vector2f(RandomRange(start, end), RandomRange(start, end));
	}

	float Random::GetRandomAngle()
	{
		return (float)rand() / RAND_MAX * 360.f;
	}
}