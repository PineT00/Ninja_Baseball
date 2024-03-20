#pragma once
#include "GameObject.h"

class SpriteGo : public GameObject
{
protected:
	sf::Sprite sprite;
	sf::RectangleShape bound;
	std::string textureId;

	SpriteGo(const SpriteGo&) = delete;
	SpriteGo(SpriteGo&&) = delete;
	SpriteGo& operator=(const SpriteGo&) = delete;
	SpriteGo& operator=(SpriteGo&&) = delete;

	bool hasHitBox = false;
public:
	sf::Shader* shader;

	SpriteGo(const std::string& name = "");
	sf::Texture* GetTexture() { return const_cast<sf::Texture*>(sprite.getTexture()); }
	void SetTexture(const sf::Texture& texture);
	void SetTexture(const std::string& textureId);

	void SetTextureRect(const sf::IntRect& rect);

	void Translate(const sf::Vector2f& delta) override;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float rot) override;
	void SetOrigin(Origins preset)			  override;
	void SetOrigin(const sf::Vector2f& origin)override;

	void SetScale(const sf::Vector2f& scale)  override;
	void SetFlipX(bool flip)				  override;
	void SetFlipY(bool flip)				  override;

	void Init() override;
	void Reset() override;
	void Draw(sf::RenderWindow& window) override;

	sf::FloatRect GetLocalBounds() override;
	sf::FloatRect GetGlobalBounds() override;

};