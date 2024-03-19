#pragma once
#include "GameObject.h"
class Tilemap : public GameObject
{
protected :
	sf::VertexArray va;
	sf::Texture* texture;

	sf::Transform transform;
	sf::Vector2i cellCount = { 6, 8 };
	sf::Vector2f cellSize;

	std::string spriteSheetId;

	int textureElementCount = 4;

public :
	sf::Shader shader;
	Tilemap(const std::string& name = "");

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void UpdateTransfrom();
	void Translate(const sf::Vector2f& delta) override;

	void Set(const sf::Vector2i& cellCount, const sf::Vector2f& cellSize);
	void SetPosition(const sf::Vector2f& pos) override;
	void SetPosition(float x, float y) override;
	void SetRotation(float rot) override;
	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& origin) override;
	void SetScale(const sf::Vector2f& scale) override;
	void SetFlipX(bool flip) override;
	void SetFlipY(bool flip) override;

	void SetSpriteSheedId(const std::string& id);

	sf::FloatRect GetLocalBounds() override;
	sf::FloatRect GetGlobalBounds() override;

	const sf::Vector2i& GetCellCount() const { return cellCount; }
	const sf::Vector2f& GetCellSize() const { return cellSize; }
};