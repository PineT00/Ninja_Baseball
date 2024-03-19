#pragma once
#include "GameObject.h"

class TextGo : public GameObject
{
protected:
	TextGo(const TextGo&) = delete;
	TextGo(TextGo&&) = delete;
	TextGo& operator=(const TextGo&) = delete;
	TextGo& operator=(TextGo&&) = delete;

	sf::Text text;

public:
	TextGo(const std::string& name = "");
	~TextGo() override = default;

	std::string GetText() const;
	void Set(const sf::Font& font, const std::string str, size_t size, const sf::Color color);
	void SetFont(const sf::Font& font);
	void SetFont(const std::string fontId);
	void SetText(const sf::Text& text);
	void SetText(const std::string& text);
	void SetTextSize(const size_t& textSize);
	void SetFillColor(const sf::Color& color);

	void Translate(const sf::Vector2f& delta) override;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetPosition(float x, float y)		  override;
	void SetRotation(float rot) override;

	void SetOrigin(Origins preset)			  override;
	void SetOrigin(const sf::Vector2f& origin) override;

	void SetScale(const sf::Vector2f& scale)  override;
	void SetFlipX(bool flip)				  override;
	void SetFlipY(bool flip)				  override;

	void AddText(char character);

	virtual void Draw(sf::RenderWindow& window);

	sf::FloatRect GetLocalBounds() override;
	sf::FloatRect GetGlobalBounds() override;
};