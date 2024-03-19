#pragma once

namespace Utils
{
	class MyMath
	{
	public:
		static float Magnitude(sf::Vector2f vector);
		static float SqrMagnitude(const sf::Vector2f vector);
		static void Normalize(sf::Vector2f& vector); 
		static sf::Vector2f GetNormal(const sf::Vector2f& vector);
		static float Distance(const sf::Vector2f& src, const sf::Vector2f& dst);
		static float Angle(const sf::Vector2f look);
		static float Rad2Deg(float radian);
		static float Deg2Rad(float degree);
		static float Clamp(float value, float min, float max);
		static sf::Vector2f Clamp(const sf::Vector2f& vector, const sf::FloatRect& rect);
		static sf::FloatRect ResizeRect(const sf::FloatRect& rect, const sf::Vector2f& delta);
		static float Lerp(float min, float max, float deltaTime);

		static void AngleWithDirectionOffsets(const sf::Vector2f& look, sf::Vector2f& directionPlus30, sf::Vector2f& directionMinus30);
		static void GenerateDirectionVectors(const sf::Vector2f& look, std::vector<sf::Vector2f>& directions, int numVectors, float angleDifference);
		static sf::Vector2f GetDirectionFromAngle(float angleDegrees);
	};
}