#include "pch.h"
#include "Tilemap.h"

Tilemap::Tilemap(const std::string& name)
	: GameObject(name)
{
}

void Tilemap::Init()
{
	GameObject::Init();

	shader.loadFromFile("magenta_to_transparent.frag", sf::Shader::Fragment);
	SetSpriteSheedId("graphics/Strikers1945/Arcade-Strikers1945II-TitleScreen.png");
	Set(cellCount, { 256, 256 });
	SetOrigin(Origins::MC);
}

void Tilemap::Release()
{
	GameObject::Release();
}

void Tilemap::Reset()
{
	GameObject::Reset();
}

void Tilemap::Update(float dt)
{
	GameObject::Update(dt);
}

void Tilemap::Set(const sf::Vector2i& count, const sf::Vector2f& size)
{
	this->cellCount = count;
	this->cellSize = size;

	va.clear();
	va.setPrimitiveType(sf::Quads);
	va.resize(count.x * count.y * 4);
	
	sf::Vector2f posOffsets[4] =
	{
		{ 0, 0 },
		{ size.x, 0 },
		{ size.x, size.y },
		{ 0, size.y }
	};

	sf::Vector2f textureCoord0[4] =
	{
		{ 0, 0 },
		{ cellSize.x, 0 },
		{ cellSize.x, cellSize.y },
		{ 0, cellSize.y }
	};

	for (int row = 0; row < count.y; ++row)
	{
		for (int col = 0; col < count.x; ++col)
		{
			int textureIndex = Utils::Random::RandomRange(0, 3);

			int quadIndex = row * count.x + col;
			sf::Vector2f quadPos(size.x * col, size.y * row);

			for (int vertex = 0; vertex < 4; ++vertex)
			{
				int vertexIndex = (quadIndex * 4) + vertex;

				va[vertexIndex].position = quadPos + posOffsets[vertex];
				va[vertexIndex].texCoords = textureCoord0[vertex];
				va[vertexIndex].texCoords.y += textureIndex * cellSize.y;
			}
		}
	}

	this->cellCount = count;
	this->cellSize = size;

	va.clear();
	va.setPrimitiveType(sf::Quads);
	va.resize(count.x * count.y * 4); 

	//sf::Vector2f posOffsets[4] =
	//{
	//	{ 0, 0 },
	//	{ size.x, 0 },
	//	{ size.x, size.y },
	//	{ 0, size.y }
	//};

	//sf::Vector2f textureCoord0[4] =
	//{
	//	{ 0, 0 },
	//	{ cellSize.x, 0 },
	//	{ cellSize.x, cellSize.y },
	//	{ 0, cellSize.y }
	//};

	for (int row = 0; row < count.y; ++row)
	{
		for (int col = 0; col < count.x; ++col)
		{
			int textureIndex = Utils::Random::RandomRange(0, 3);

			if (row == 0 || row == count.y - 1 || col == 0 || col == count.x - 1)
			{
				textureIndex = 3;
			}

			int quadIndex = row * count.x + col;
			sf::Vector2f quadPos(size.x * col, size.y * row);

			for (int vertex = 0; vertex < 4; ++vertex)
			{
				int vertexIndex = (quadIndex * 4) + vertex;

				va[vertexIndex].position = quadPos + posOffsets[vertex];
				va[vertexIndex].texCoords = textureCoord0[vertex];
				va[vertexIndex].texCoords.y += textureIndex * cellSize.y;
			}
		}
	}
}

void Tilemap::Translate(const sf::Vector2f& delta)
{
	GameObject::Translate(delta);
	UpdateTransfrom();
}

void Tilemap::Draw(sf::RenderWindow& window)
{
	GameObject::Draw(window);

	sf::RenderStates state;
	state.texture = texture;
	state.transform = transform;
	window.draw(va, state);
}

void Tilemap::UpdateTransfrom()
{
	transform = sf::Transform::Identity;

	float scaleX = isFlipX ? -scale.x : scale.x;
	float scaleY = isFlipY ? -scale.y : scale.y;

	transform.scale(scaleX, scaleY, position.x , position.y);
	transform.rotate(rotation, position.x, position.y);
	transform.translate(position - origin);
}

void Tilemap::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	UpdateTransfrom();
}

void Tilemap::SetPosition(float x, float y)
{
	GameObject::SetPosition({x, y});
	UpdateTransfrom();
}

void Tilemap::SetOrigin(Origins preset)
{
	if (preset == Origins::CUSTOM)
		return;
	
	originPreset = preset;
	sf::FloatRect bound = va.getBounds();
	origin.x = bound.width * ((int)originPreset % 3) * 0.5f;
	origin.y = bound.height * ((int)originPreset / 3) * 0.5f;
	UpdateTransfrom();
}

void Tilemap::SetOrigin(const sf::Vector2f& origin)
{
	originPreset = Origins::CUSTOM;
	this->origin = origin;
	UpdateTransfrom();
}

void Tilemap::SetScale(const sf::Vector2f& scale)
{
	GameObject::SetScale(scale);
	UpdateTransfrom();
}

void Tilemap::SetFlipX(bool flip)
{
	GameObject::SetFlipX(flip);
	UpdateTransfrom();
}

void Tilemap::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	UpdateTransfrom();
}

void Tilemap::SetFlipY(bool flip)
{
	GameObject::SetFlipY(flip);
	UpdateTransfrom();
}

void Tilemap::SetSpriteSheedId(const std::string& id)
{
	this->spriteSheetId = id;
	this->texture = TEXTURE_MANAGER.GetResource(spriteSheetId);
}

sf::FloatRect Tilemap::GetLocalBounds()
{
	sf::FloatRect bounds = va.getBounds();
	bounds.left = origin.x;
	bounds.top = origin.y;

	return bounds;
}

sf::FloatRect Tilemap::GetGlobalBounds()
{
	sf::FloatRect bounds = va.getBounds();
	return transform.transformRect(bounds);
}