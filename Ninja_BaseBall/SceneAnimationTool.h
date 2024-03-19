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

	std::vector<sf::FloatRect> selectedAreas;
	std::wstring atlasPath;
	
	sf::Font font;
	sf::RectangleShape editorBorder;
	sf::RectangleShape textureBorder;
	sf::Vector2f windowSize;
	sf::Vector2f leftDragStartPos;
	sf::Vector2f rightDragStartPos;
	sf::Vector2f lastMousePos;

	bool isAtlasPath = false;
	bool firstTextureLoad = true;
	bool isLeftDragging = false;
	bool isRightDragging = false;

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
	bool IsWithinWorldView(const sf::Vector2f& point);
};