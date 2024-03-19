#pragma once
#include "Scene.h"

class Button;
class SpriteGo;

class SceneAnimationTool : public Scene
{
protected:
	SceneAnimationTool(const SceneAnimationTool&) = delete;
	SceneAnimationTool(SceneAnimationTool&&) = delete;
	SceneAnimationTool& operator=(const SceneAnimationTool&) = delete;
	SceneAnimationTool& operator=(SceneAnimationTool&&) = delete;

	Button* buttonLoadAtlas = nullptr;
	SpriteGo* spriteSheet = nullptr;

	std::wstring atlasPath;

	sf::View workingView;
	sf::Font font;
	sf::Vector2f windowSize;

	bool isAtlasPath = false;

public:
	SceneAnimationTool(SceneIDs id);
	~SceneAnimationTool() override = default;

	void Init() override;
	void Release() override;
	virtual void Reset();
	void Enter() override;
	void Exit() override;
	void Update(float dt) override;
	void UpdateAwake(float dt);
	void UpdateGame(float dt);
	void UpdateGameover(float dt);
	void UpdatePause(float dt);
	void Draw(sf::RenderWindow& window);

	void SetAtlasPath(const std::wstring& str);

	GameStatus GetStatus() const { return this->status; }
	void SetStatus(GameStatus newStatus);
};