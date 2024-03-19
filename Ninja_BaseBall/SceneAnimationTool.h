#pragma once
#include "Scene.h"

class Button;
class SpriteGo;
class TextGo;
class InputField;

class SceneAnimationTool : public Scene
{
protected:
	SceneAnimationTool(const SceneAnimationTool&) = delete;
	SceneAnimationTool(SceneAnimationTool&&) = delete;
	SceneAnimationTool& operator=(const SceneAnimationTool&) = delete;
	SceneAnimationTool& operator=(SceneAnimationTool&&) = delete;

	Button* buttonLoadAtlas = nullptr;
	Button* buttonSaveAnimation = nullptr;
	Button* buttonStop = nullptr;
	Button* buttonPlay = nullptr;

	SpriteGo* spriteSheet = nullptr;
	TextGo* textFPS;
	InputField* inputfieldFPS = nullptr;

	std::wstring atlasPath;
	
	sf::Font font;
	sf::Vector2f windowSize;

	bool isAtlasPath = false;
	bool firstTextureLoad = true;

	// 드래그 처리를 위한 멤버 변수 추가
	bool isLeftDragging = false;
	sf::Vector2f leftDragStartPos;
	std::vector<sf::FloatRect> selectedAreas;

	bool isRightDragging = false;
	sf::Vector2f rightDragStartPos;
	sf::Vector2f lastMousePos;

	float zoom = 0.f;

public:
	SceneAnimationTool(SceneIDs id);
	~SceneAnimationTool() override = default;

	void Init() override;
	void Release() override;
	virtual void Reset();
	void Enter() override;
	void Exit() override;
	void Update(float dt) override;
	void UpdateEvent(const sf::Event& event) override;
	void UpdateAwake(float dt);
	void UpdateGame(float dt);
	void UpdateGameover(float dt);
	void UpdatePause(float dt);
	void Draw(sf::RenderWindow& window);

	bool IsAtlasPath() const { return isAtlasPath; }
	void SetIsAtlasPath(bool isAtlasPath) { this->isAtlasPath = isAtlasPath; }
	void SetAtlasPath(const std::wstring& str);

	GameStatus GetStatus() const { return this->status; }
	void SetStatus(GameStatus newStatus);
};