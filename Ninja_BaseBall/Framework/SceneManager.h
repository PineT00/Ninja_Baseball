#pragma once
#include "Singleton.h"

class Scene; 

enum class SceneIDs
{
	None = -1,
	SceneTitle,
	SceneGame,
	SceneDev1,
	TestScene,
	COUNT,
};

class SceneManager : public Singleton<SceneManager>
{
	friend class Singleton<SceneManager>;

protected:
	SceneManager(const SceneManager&)			 = delete;
	SceneManager(SceneManager&&)				 = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager& operator=(SceneManager&&)		 = delete;

	std::vector<Scene*> scenes; 
	SceneIDs startScene = SceneIDs::TestScene;
	SceneIDs currentScene = startScene;
	SceneIDs nextScene = SceneIDs::None;

	std::vector<std::string> TextureResourceNames = { };
	std::vector<std::string> FontResourceNames = {};
	std::vector<std::string> SoundResourceNames = {};

	bool isDeveloperMode = false; //개발자 모드용 변수

public:

	SceneManager() = default;
	virtual ~SceneManager();
	
	void Init();
	void Release();

	Scene* GetCurrentScene() { return scenes[(int)currentScene]; }
	Scene* GetScene(SceneIDs id) { return scenes[(int)id]; }
	
	bool GetDeveloperMode() { return isDeveloperMode; } //개발자 모드용 메소드

	void LoadAllResources();
	void ChangeScene(SceneIDs id);
	bool Update(float dt);
	void UpdateEvent(const sf::Event& event);
	void LateUpdate(float dt);
	void FixeUpdate(float dt);
	void Draw(sf::RenderWindow& window);
};

#define SCENE_MANAGER (Singleton<SceneManager>::Instance())