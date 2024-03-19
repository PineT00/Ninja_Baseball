#include "pch.h"
#include "SpriteGo.h"

SpriteGo::SpriteGo(const std::string& name)
	: GameObject(name)
{
}

void SpriteGo::SetTexture(const sf::Texture& texture)
{
	sprite.setTexture(texture);
}

void SpriteGo::SetTexture(const std::string& textureId)
{
	this->textureId = textureId;
	SetTexture(*TEXTURE_MANAGER.GetResource(textureId));
}

void SpriteGo::SetTextureRect(const sf::IntRect& rect)
{
	sprite.setTextureRect(rect);
}

void SpriteGo::Translate(const sf::Vector2f& delta)
{
	position += delta;
	sprite.setPosition(position);
}

void SpriteGo::SetPosition(const sf::Vector2f& pos)
{
	this->position = pos;

	sprite.setPosition(this->position);
}

void SpriteGo::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	sprite.setRotation(rot);
}

void SpriteGo::SetOrigin(Origins preset)
{
	if (preset == Origins::CUSTOM)
	{
		preset == Origins::TL;
	}

	originPreset = preset;
	Utils::Origin::SetOrigin(sprite, originPreset);
}

void SpriteGo::SetOrigin(const sf::Vector2f& origin)
{
	originPreset = Origins::CUSTOM;
	this->origin = origin;
	sprite.setOrigin(this->origin);
}

void SpriteGo::SetScale(const sf::Vector2f& scale)
{
	sf::Vector2f spriteScale = this->scale = scale;

	if (isFlipX)
	{
		spriteScale.x = -spriteScale.x;
	}

	if (isFlipY)
	{
		spriteScale.y = -spriteScale.y;
	}

	sprite.setScale(spriteScale);
}

void SpriteGo::SetFlipX(bool flip)
{
	if (isFlipX == flip) return;
	isFlipX = flip;

	SetScale(scale);
}

void SpriteGo::SetFlipY(bool flip)
{
	if (isFlipY == flip) return;
	isFlipY = flip;

	SetScale(scale);
}

void SpriteGo::Init()
{
	shader = new sf::Shader();
	shader->loadFromFile("transparent.frag", sf::Shader::Fragment);
}

void SpriteGo::Reset()
{
	sprite.setTexture(*TEXTURE_MANAGER.GetResource(textureId));
}

void SpriteGo::Draw(sf::RenderWindow& window)
{
	window.draw(sprite, shader);
}


sf::FloatRect SpriteGo::GetLocalBounds()
{
	return sprite.getLocalBounds();
}

sf::FloatRect SpriteGo::GetGlobalBounds()
{
	return sprite.getGlobalBounds();
}