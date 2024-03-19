#pragma once
#include "MyMath.h"

namespace Utils
{
	class Vector2
	{
	public:
		static sf::Vector2f Min(const sf::Vector2f& vector1, const sf::Vector2f& vector2);
		static sf::Vector2f Max(const sf::Vector2f& vector1, const sf::Vector2f& vector2);
		static float SignedAngle(const sf::Vector2f& from, const sf::Vector2f& to);
		static float Dot(const sf::Vector2f& vector1, const sf::Vector2f& vector2);
		static sf::Vector2f Lerp(const sf::Vector2f& src, const sf::Vector2f& dst, float deltaTime);
		static sf::Vector2f MoveTowards(const sf::Vector2f& current, const sf::Vector2f& target, float maxDistanceDelta);
		static sf::Vector2f Perpendicular(const sf::Vector2f& vector);
		static sf::Vector2f Reflect(const sf::Vector2f& inDirection, const sf::Vector2f& inNormal);
		static sf::Vector2f Scale(const sf::Vector2f& vector1, const sf::Vector2f& vector2);
		static sf::Vector2f ClampMagnitude(sf::Vector2f vector, float maxLength);
	};
}
