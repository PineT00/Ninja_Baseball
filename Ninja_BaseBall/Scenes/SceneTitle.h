#pragma once

class SpriteGo;
class TextGo;

class SceneTitle : public Scene
{
protected:
	SceneTitle(const SceneTitle&) = delete;
	SceneTitle(SceneTitle&&) = delete;
	SceneTitle& operator=(const SceneTitle&) = delete;
	SceneTitle& operator=(SceneTitle&&) = delete;

	sf::Font font;
	sf::Vector2f windowSize;

	SpriteGo* titleName = nullptr;
	SpriteGo* insertCoin = nullptr;
	SpriteGo* backgroundBrick = nullptr;
	SpriteGo* backgroundShadow = nullptr;

	std::vector<TextGo*> textDescriptions;
	std::vector<std::string> descriptions = {"1 COIN 1 PLAY", "C. 1993 IREM AMERICA CORP.", "CREDIT 0"};

	float blinkTime = 0.7f;
	float time = 0.f;

public:
	SceneTitle(SceneIDs id);
	~SceneTitle() override = default;

	void Init() override;
	void Release() override;
	void Enter() override;
	void Exit() override;
	void Update(float dt) override;
	void BlinkIntro(float blinkTime);
};