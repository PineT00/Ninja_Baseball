#pragma once
#include "Singleton.h"

class Scene; 

enum class SceneIDs
{
	None = -1,
	SceneTitle,
	SceneGame,
	SceneAnimationTool,
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

	//SceneIDs startScene = SceneIDs::SceneAnimationTool;
	SceneIDs startScene = SceneIDs::SceneTitle;
	SceneIDs currentScene = startScene;
	SceneIDs nextScene = SceneIDs::None;

	std::vector<std::string> TextureResourceNames = {"graphics/4_Boss/boss_attacked(no_arm).bmp", "graphics/4_Boss/boss_attacked(no_one_arm).bmp", "graphics/4_Boss/boss_attacked(no_wing).bmp", "graphics/4_Boss/boss_attacked(no_wing_prop).bmp",
	"graphics/4_Boss/boss_attacked.bmp", "graphics/4_Boss/boss_gun_ready(no_wing).bmp", "graphics/4_Boss/boss_idle(basic).bmp", "graphics/4_Boss/boss_idle(no_one_arm).bmp", "graphics/4_Boss/boss_idle(no_wing).bmp",  
		"graphics/4_Boss/boss_death.bmp", "graphics/4_Boss/boss_final.bmp", "graphics/4_Boss/boss_gun(no_wing).bmp", "graphics/4_Boss/boss_gun_prop(no_wing).bmp", "graphics/4_Boss/boss_idle(no_wing_prop).bmp", "graphics/4_Boss/boss_onetwo.bmp", "graphics/4_Boss/boss_straight(no_one_arm).bmp" 
	, "graphics/4_Boss/boss_straight(no_wing).bmp" , "graphics/4_Boss/boss_straight(no_wing_prop).bmp" , "graphics/4_Boss/boss_straight.bmp" , "graphics/4_Boss/boss_straight2.bmp" , "graphics/4_Boss/boss_upper_cut(no_wing).bmp" , "graphics/4_Boss/boss_upper_cut(no_wing_prop).bmp",
	"graphics/4_Boss/boss_upper_cut.bmp", "graphics/4_Boss/boss_wind.bmp", "graphics/4_Boss/boss_windEffect.bmp", "graphics/4_Boss/sboss_onetwo.bmp", "graphics/4_Boss/tboss_onetwo.bmp"};
	std::vector<std::string> FontResourceNames = {};
	std::vector<std::string> SoundResourceNames = {};

	bool isDeveloperMode = false;

public:

	SceneManager() = default;
	virtual ~SceneManager();
	
	void Init();
	void Release();

	Scene* GetCurrentScene() { return scenes[(int)currentScene]; }
	Scene* GetScene(SceneIDs id) { return scenes[(int)id]; }
	
	bool GetDeveloperMode() { return isDeveloperMode; }

	void LoadAllResources();
	void ChangeScene(SceneIDs id);
	bool Update(float dt);
	void UpdateEvent(const sf::Event& event);
	void LateUpdate(float dt);
	void FixeUpdate(float dt);
	void Draw(sf::RenderWindow& window);
};

#define SCENE_MANAGER (Singleton<SceneManager>::Instance())