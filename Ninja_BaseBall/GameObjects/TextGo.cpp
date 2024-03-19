#include "pch.h"
#include "TextGo.h"

TextGo::TextGo(const std::string& name)
	: GameObject(name)
{
}

std::string TextGo::GetText() const
{
	return text.getString();
}

void TextGo::Set(const sf::Font& font, const std::string str, size_t size, const sf::Color color)
{
	SetFont(font);
	text.setString(str);
	text.setCharacterSize(size);
	text.setFillColor(color);
}

void TextGo::SetText(const std::string& text)
{
	this->text.setString(text);
	SetOrigin(originPreset);
}

void TextGo::SetText(const sf::Text& text)
{
	this->text = text;
	SetOrigin(originPreset);
}

void TextGo::SetFont(const sf::Font& font)
{
	this->text.setFont(font);
}

void TextGo::SetFont(const std::string fontId)
{
	SetFont(*ResourceManager<sf::Font>::Instance().GetResource(fontId));
}

void TextGo::SetFillColor(const sf::Color& color)
{
	this->text.setFillColor(color);
}

void TextGo::Translate(const sf::Vector2f& delta)
{
	position += delta;
	text.setPosition(position);
}

void TextGo::SetPosition(const sf::Vector2f& pos)
{
	this->position = pos;
	text.setPosition(this->position);
}

void TextGo::SetPosition(float x, float y)
{
	this->position.x = x;
	this->position.y = y;
	text.setPosition({ x, y });
}

void TextGo::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	text.setRotation(rot);
}

void TextGo::SetOrigin(Origins preset)
{
	if (preset == Origins::CUSTOM)
	{
		preset = Origins::TL;
	}

	originPreset = preset;
	Utils::Origin::SetOrigin(text, originPreset);
}

void TextGo::SetOrigin(const sf::Vector2f& origin)
{
	originPreset = Origins::CUSTOM;
	this->origin = origin;
	text.setOrigin(this->origin);
}

void TextGo::SetScale(const sf::Vector2f& scale)
{
	sf::Vector2f textScale = this->scale = scale;

	if (isFlipX)
	{
		textScale.x = -textScale.x;
	}

	if (isFlipY)
	{
		textScale.y = -textScale.y;
	}

	text.setScale(textScale);
}

void TextGo::SetFlipX(bool flip)
{
	if (isFlipX == flip) return;
	isFlipX = flip;

	SetScale(scale);
}

void TextGo::SetFlipY(bool flip)
{
	if (isFlipY = flip) return;
	isFlipY = flip;

	SetScale(scale);
}

void TextGo::AddText(char character)
{
	text.setString(text.getString() + character);
	SetOrigin(originPreset);
}

void TextGo::SetTextSize(const size_t& textSize)
{
	this->text.setCharacterSize(textSize);
}

void TextGo::Draw(sf::RenderWindow& window)
{
	window.draw(text);
}

sf::FloatRect TextGo::GetLocalBounds()
{
	return text.getLocalBounds();
}

sf::FloatRect TextGo::GetGlobalBounds()
{
	return text.getGlobalBounds();
}