#pragma once
class GameObject
{
protected:
	GameObject(const GameObject&) = delete;
	GameObject(GameObject&&) = delete;
	GameObject& operator=(const GameObject&) = delete;
	GameObject& operator=(GameObject&&) = delete;

	Origins originPreset = Origins::TL;
	std::string name = "";

	sf::Vector2f origin = { 0.f, 0.f };
	sf::Vector2f position = { 0.f, 0.f };
	sf::Vector2f scale = { 1.f, 1.f };

	float rotation = 0.f;

	bool isActive = true;
	bool isFlipX = false;
	bool isFlipY = false;
	bool isFocused = false;

	int sortLayer = 0; // �ٸ� ���̾� �� ����
	int sortOrder = 0; // ���� ���̾� �� ����

public:
	static bool CompareDrawOrder(const GameObject& lhs, const GameObject& rhs);
	static bool CompareDrawOrder(const GameObject* lhs, const GameObject* rhs);

	GameObject(const std::string& name = "");
	virtual ~GameObject();

	bool GetActive() const { return this->isActive; }
	virtual void SetActive(bool active) { this->isActive = active; }

	virtual void Translate(const sf::Vector2f& delta) { position += delta; }

	sf::Vector2f GetPosition() const { return this->position; }
	virtual void SetPosition(const sf::Vector2f& pos) { this->position = pos; }
	virtual void SetPosition(float x, float y);

	sf::Vector2f GetOrigin() const { return this->origin; }
	virtual void SetOrigin(Origins preset);
	virtual void SetOrigin(const sf::Vector2f& origin)
	{
		originPreset = Origins::CUSTOM;
		this->origin = origin;
	}

	virtual void Init();
	virtual void Release();
	virtual void Reset();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void FixedUpdate(float dt);
	virtual void Draw(sf::RenderWindow& window);

	float  GetRotation() const { return this->rotation; }
	virtual void SetRotation(float rot) { this->rotation = rot; }

	sf::Vector2f GetScale() const { return this->scale; }
	virtual void SetScale(const sf::Vector2f& scale);

	virtual bool GetFlipX() const { return this->isFlipX; }
	virtual void SetFlipX(bool flip) { this->isFlipX = flip; }
	virtual bool GetFlipY() const { return this->isFlipY; }
	virtual void SetFlipY(bool flip) { this->isFlipY = flip; }

	std::string GetName() const { return this->name; }
	virtual void SetName(std::string& name) { this->name = name; }

	int GetSortLayer() const { return this->sortLayer; }
	void SetSortLayer(int layer) { this->sortLayer = layer; }
	int GetSortOrder() { return this->sortOrder; }
	void SetSortOrder(int order) { this->sortOrder = order; }

	virtual sf::FloatRect GetLocalBounds() { return sf::FloatRect(); }
	virtual sf::FloatRect GetGlobalBounds() { return sf::FloatRect(position, { 0.f,0.f }); }

	virtual void SetFocused(bool focuse) { this->isFocused = focuse; }
	virtual bool IsFocused() { return this->isFocused; }

	bool operator>(const GameObject& rhs) const;
	bool operator<(const GameObject& rhs) const;
};