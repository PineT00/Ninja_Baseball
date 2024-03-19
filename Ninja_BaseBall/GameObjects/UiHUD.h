#pragma once
#include "GameObject.h"
#include "TextGo.h"	
#include "SpriteGo.h"

class TextGo;
class SpriteGo;
class SceneGame;

class UiHUD : public GameObject
{
protected:
	UiHUD(const UiHUD&) = delete;
	UiHUD(UiHUD&&) = delete;
	UiHUD& operator=(const UiHUD&) = delete;
	UiHUD& operator=(UiHUD&&) = delete;
	
	sf::Vector2f referenceResolution = { 960.f, 540.f };
	sf::Vector2f resolution = referenceResolution;
	sf::Vector2i windowSize;

	SceneGame* sceneGame = nullptr;

public:
	UiHUD(const std::string& name = "hud");
	~UiHUD() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};