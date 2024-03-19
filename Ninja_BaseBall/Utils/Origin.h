#pragma once

namespace Utils
{
	class Origin
	{
	public:
		static sf::Vector2f SetOrigin(sf::Transformable& obj, Origins originPreset, const sf::FloatRect& rect);
		static sf::Vector2f SetOrigin(sf::Sprite& obj, Origins originPreset);
		static sf::Vector2f SetOrigin(sf::Text& obj, Origins originPreset);
		static sf::Vector2f SetOrigin(sf::Shape& obj, Origins originPreset);
	};
}