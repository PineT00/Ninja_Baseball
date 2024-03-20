#pragma once
#include "Scene.h"
#include "Animator.h"

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

	std::vector<sf::FloatRect> selectedAreas;
	std::vector<Origins> selectedAreasPivot;
	std::vector<Button*> originButtons;
	std::vector<Button*> loopButtons;
	std::vector<std::string> originButtonsText = {"TL", "TC", "TR", "ML", "MC", "MR","BL", "BC", "BR"};
	std::vector<std::string> loopButtonsText = {"Single", "Loop", "Ping Pong"};

	SpriteGo* spriteSheet = nullptr;
	TextGo* textFPS;
	InputField* inputfieldFPS = nullptr;
	AnimationLoopType selectedAreaLoopType;

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
	std::wstring GetAtlasPath() const { return atlasPath; }
	void SetAtlasPath(const std::wstring& str);

	std::vector<sf::FloatRect>& GetSelectedAreas() { return selectedAreas; }
	std::vector<Origins>& GetSelectedAreasPivot() { return selectedAreasPivot; }
	AnimationLoopType& GetSelectedAreaLoopType() { return selectedAreaLoopType; }

	InputField* GetFPS() const { return inputfieldFPS; }
	
	GameStatus GetStatus() const { return this->status; }
	void SetStatus(GameStatus newStatus);
	bool IsWithinWorldView(const sf::Vector2f& point);
};