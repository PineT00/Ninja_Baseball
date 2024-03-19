#include "pch.h"
#include "MyMath.h"

namespace Utils
{
	float MyMath::Magnitude(const sf::Vector2f vector)
	{
		return std::sqrt(vector.x * vector.x + vector.y * vector.y);
	}

	float MyMath::SqrMagnitude(const sf::Vector2f vector)
	{
		return vector.x * vector.x + vector.y * vector.y;
	}

	void MyMath::Normalize(sf::Vector2f& vector)
	{
		float magnitude = Magnitude(vector);

		if (magnitude != 0.f)
		{
			vector /= magnitude;
		}
	}

	sf::Vector2f MyMath::GetNormal(const sf::Vector2f& vector)
	{
		float magnitude = Magnitude(vector);

		if (magnitude == 0.f)
		{
			return vector;
		}
		return vector / Magnitude(vector);
	}

	float MyMath::Distance(const sf::Vector2f& src, const sf::Vector2f& dst)
	{
		return Magnitude(dst - src);
	}

	float MyMath::Angle(const sf::Vector2f look)
	{
		return Rad2Deg(std::atan2f(look.y, look.x));
	}

	float MyMath::Rad2Deg(float radian)
	{
		return radian * (180.f / M_PI);
	}

	float MyMath::Deg2Rad(float degree)
	{
		return degree * (M_PI / 180.f);
	}

	float MyMath::Clamp(float value, float min, float max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	sf::Vector2f MyMath::Clamp(const sf::Vector2f& vector, const sf::FloatRect& rect)
	{
		return sf::Vector2f(Clamp(vector.x, rect.left, rect.left + rect.width), Clamp(vector.y, rect.top, rect.top + rect.height));
	}

	sf::FloatRect MyMath::ResizeRect(const sf::FloatRect& rect, const sf::Vector2f& delta)
	{
		sf::FloatRect newRect = rect;
		newRect.width += delta.x;
		newRect.height += delta.y;

		newRect.left -= delta.x * 0.5f;
		newRect.top -= delta.y * 0.5f;

		return newRect;
	}

	float MyMath::Lerp(float src, float dst, float deltaTime)
	{
		deltaTime = Utils::MyMath::Clamp(deltaTime, 0.f, 1.f);

		float v = src + (dst - src) * deltaTime;

		return v;
	}
	void MyMath::AngleWithDirectionOffsets(const sf::Vector2f& look, sf::Vector2f& directionPlus30, sf::Vector2f& directionMinus30)
	{
		float angle = std::atan2(look.y, look.x);
		float anglePlus30 = angle + Deg2Rad(30.0f);
		float angleMinus30 = angle - Deg2Rad(30.0f);

		directionPlus30 = sf::Vector2f(std::cos(anglePlus30), std::sin(anglePlus30));
		directionMinus30 = sf::Vector2f(std::cos(angleMinus30), std::sin(angleMinus30));
	}
	void MyMath::GenerateDirectionVectors(const sf::Vector2f& look, std::vector<sf::Vector2f>& directions, int numVectors, float angleDifference)
	{
		float baseAngle = std::atan2(look.y, look.x);
		int halfNum = (numVectors - 1) / 2;

		for (int i = -halfNum; i <= halfNum; ++i)
		{
			float angle = baseAngle + Deg2Rad(angleDifference * i);
			sf::Vector2f direction(std::cos(angle), std::sin(angle));
			directions.push_back(direction);
		}
	}
	sf::Vector2f MyMath::GetDirectionFromAngle(float angleDegrees)
	{
		float angleRadians = Deg2Rad(angleDegrees);
		return sf::Vector2f(std::cos(angleRadians), std::sin(angleRadians));

	}
}
