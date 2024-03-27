#pragma once
#include "GameObject.h"
#include "TextGo.h"	
#include "SpriteGo.h"

class TextGo;
class SpriteGo;
class SceneGame;
class SceneDev1;
class Player;

class UiHUD : public GameObject
{
public:
	Player* player = nullptr;

	SpriteGo* player1_Frame = nullptr;
	SpriteGo* player1_Name = nullptr;
	sf::Sprite player1_Portrait;
	TextGo player1_Score;
	TextGo player1_Life;

	SpriteGo player1_hpRedBack;
	SpriteGo player1_hpBar;

	Animator uiAnimator;

	int score = 0;
	int life = 0;

	sf::Font font;

	float player1_prevHP;

	int gameOverCount = 10;

	TextGo gameOver;
	TextGo gameOverTimer;
	TextGo insertCoin;

protected:
	UiHUD(const UiHUD&) = delete;
	UiHUD(UiHUD&&) = delete;
	UiHUD& operator=(const UiHUD&) = delete;
	UiHUD& operator=(UiHUD&&) = delete;
	
	sf::Vector2f referenceResolution = { 960.f, 540.f };
	sf::Vector2f resolution = referenceResolution;
	sf::Vector2i windowSize;

	std::string formatFps = "FPS: ";
	TextGo textFps;

	//SceneGame* sceneGame = nullptr;
	//SceneDev1* sceneDev1 = nullptr;



public:
	UiHUD(const std::string& name = "hud");
	~UiHUD() override = default;

	void SetFps(int fps);

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void GameOverCount();
};