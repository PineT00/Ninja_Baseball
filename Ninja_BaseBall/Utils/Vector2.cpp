#include "pch.h"
#include "Vector2.h"

namespace Utils
{
	sf::Vector2f Vector2::Min(const sf::Vector2f& vector1, const sf::Vector2f& vector2)
	{
		return sf::Vector2f(std::min(vector1.x, vector2.x), std::min(vector1.y, vector2.y));
	}

	sf::Vector2f Vector2::Max(const sf::Vector2f& vector1, const sf::Vector2f& vector2)
	{
		return sf::Vector2f(std::max(vector1.x, vector2.x), std::max(vector1.y, vector2.y));
	}
	float Vector2::SignedAngle(const sf::Vector2f& from, const sf::Vector2f& to)
	{
		float unsignedAngle = std::acos(Dot(from, to)) / (Utils::MyMath::Magnitude(from) * Utils::MyMath::Magnitude(to));
		float sign = from.x * to.y - to.x * from.y < 0.f ? -1.f : 1.f;

		return Utils::MyMath::Rad2Deg(sign * unsignedAngle);
	}
	float Vector2::Dot(const sf::Vector2f& vector1, const sf::Vector2f& vector2)
	{
		return vector1.x * vector2.x + vector1.y * vector2.y;
	}

	sf::Vector2f Vector2::Lerp(const sf::Vector2f& src, const sf::Vector2f& dst, float deltaTime)
	{
		return sf::Vector2f(src.x + (dst.x - src.x) * deltaTime, src.y + (dst.y - src.y) * deltaTime);
	}

	sf::Vector2f Vector2::MoveTowards(const sf::Vector2f& current, const sf::Vector2f& target, float maxDistanceDelta)
	{
		sf::Vector2f direction = target - current;
		float distance = Utils::MyMath::Magnitude(direction);
		if (distance <= maxDistanceDelta || distance == 0.f)
		{
			return target;
		}
		return current + (direction / distance) * maxDistanceDelta;
	}

	sf::Vector2f Vector2::Perpendicular(const sf::Vector2f& vector)
	{
		return sf::Vector2f(-vector.y, vector.x);
	}

	sf::Vector2f Vector2::Reflect(const sf::Vector2f& inDirection, const sf::Vector2f& inNormal)
	{
		return sf::Vector2f(inDirection - 2.f * Dot(inDirection, inNormal) * inNormal);
	}

	sf::Vector2f Vector2::Scale(const sf::Vector2f& vector1, const sf::Vector2f& vector2)
	{
		return sf::Vector2f(vector1.x * vector2.x, vector1.y * vector2.y);
	}
	sf::Vector2f Vector2::ClampMagnitude(sf::Vector2f vector, float maxLength)
	{
		float sqrtMag = Utils::MyMath::SqrMagnitude(vector);
		if (sqrtMag > maxLength * maxLength)
		{
			sf::Vector2f normalized = Utils::MyMath::GetNormal(vector);
			return sf::Vector2f(normalized.x * maxLength, normalized.y * maxLength);
		}
		return vector;
	}
}