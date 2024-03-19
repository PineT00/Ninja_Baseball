#pragma once
class GameObject;

class Scene
{
protected:
	Scene(const Scene&) = delete;
	Scene(Scene&&) = delete;
	Scene& operator=(const Scene&) = delete;
	Scene& operator=(Scene&&) = delete;

	SceneIDs id;
	std::unordered_map<size_t, std::vector<GameObject*>> gameObjectsMap;

	std::list<GameObject*> gameObjects;
	std::list<GameObject*> uiGameObjects;

	std::list<GameObject*> resortingGameObjects;
	std::list<GameObject*> removeGameObjects;

	sf::View worldView;
	sf::View uiView;

	ResourceManager<sf::Texture>& textureManager;
	ResourceManager<sf::Font>& fontManager;
	ResourceManager<sf::SoundBuffer>& soundManager;

	GameStatus status = GameStatus::Game;

public:
	Scene(SceneIDs id);
	virtual ~Scene() = default;

	virtual void Init();
	virtual void Release();
	virtual void Enter();
	virtual void Exit();
	virtual void Update(float dt);
	virtual void UpdateEvent(const sf::Event& event);
	virtual void LateUpdate(float dt);
	virtual void FixedUpdate(float dt);
	virtual void Draw(sf::RenderWindow& window);

	virtual GameObject* FindGameObject(const std::string& name, Layers layer = Layers::EveryThing);
	virtual std::list<GameObject*>& FindAllGameObjectwithName(const std::string& name, Layers layer = Layers::EveryThing);
	virtual int FindAll(const std::string& name, std::list<GameObject*>& list, Layers layer = Layers::EveryThing);
	virtual GameObject* AddGameObject(GameObject* gameObject, Layers layer = Layers::World);
	virtual GameObject* AddGameObject(const std::string& name, Layers layer = Layers::World);
	virtual void RemoveGameObject(GameObject* gameObject);
	virtual void RemoveGameObject(std::string name);
	virtual void ResortGameObject(GameObject* obj);


	sf::Vector2f ScreenToWorld(sf::Vector2i screenPosition);
	sf::Vector2i WorldToScreen(sf::Vector2f worldPosition);
	sf::Vector2f ScreenToUi(sf::Vector2i screenPosition);
	sf::Vector2i UiToScreen(sf::Vector2f uiPosition);

	virtual GameStatus GetStatus() const { return this->status; }
	void SetStatus(GameStatus newStatus);
};