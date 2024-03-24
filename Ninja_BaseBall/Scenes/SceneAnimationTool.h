#pragma once
#include "Scene.h"
#include "Animator.h"

class Button;
class SpriteGo;
class TextGo;
class InputField;
class PreviewCharacter;

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
	
	// TODO : Auto Slice 기능
	Button* buttonAutoSlice = nullptr;
	Button* buttonLoadCsv = nullptr;

	std::vector<sf::FloatRect> selectedAreas;
	std::vector<Origins> selectedAreasPivot;
	std::vector<Button*> originButtons;
	std::vector<Button*> loopButtons;
	std::vector<std::string> originButtonsText = {"TL", "TC", "TR", "ML", "MC", "MR","BL", "BC", "BR", "CUSTOM"};
	std::vector<std::string> loopButtonsText = {"Single", "Loop", "Ping Pong"};
	std::vector<sf::Vector2f> customPivots;
	std::wstring atlasPath;

	std::vector<std::string> guidlines = { "Select Frame Bound", "Select Pivot"};

	SpriteGo* spriteSheet = nullptr;
	TextGo* textFPS;
	TextGo* textGuidline;
	InputField* inputfieldFPS = nullptr;
	// TODO : Auto Slice 기능
	InputField* inputfieldRow = nullptr;
	InputField* inputfieldCol = nullptr;
	PreviewCharacter* previewCharacter = nullptr;
	AnimationLoopType selectedLoopType = AnimationLoopType::Single;

	sf::Font font;
	sf::RectangleShape editorBorder;
	sf::RectangleShape textureBorder;
	sf::Vector2f windowSize;
	sf::Vector2f leftDragStartPos;
	sf::Vector2f rightDragStartPos;
	sf::Vector2f lastMousePos;
	sf::Vector2f currentMousePos;

	bool isAtlasPath = false;
	bool firstTextureLoad = true;
	bool isLeftDragging = false;
	bool isRightDragging = false;
	bool isCustomPivot = false;
	bool isLoadedFromCsv = false;

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
	void SetAtlasPath(std::wstring& str);

	std::vector<sf::FloatRect>& GetSelectedAreas() { return selectedAreas; }
	std::vector<Origins>& GetSelectedAreasPivot() { return selectedAreasPivot; }
	std::vector<sf::Vector2f>& GetCustomPivot() { return customPivots; }
	AnimationLoopType& GetSelectedLoopType() { return selectedLoopType; }

	bool IsCustomPivot() const { return isCustomPivot; }
	void SetIsCutomPivot(bool isCustom) { isCustomPivot = isCustom; }

	bool IsLoadedFromCsv() const { return isLoadedFromCsv; }
	void SetLoadedFromCsv(bool isLoadedFromCsv) { this->isLoadedFromCsv = isLoadedFromCsv; }
	void SetFPS(const std::wstring& fps);
	void SetLoopType(const AnimationLoopType looptype);

	InputField* GetFPS() const { return inputfieldFPS; }
	GameStatus GetStatus() const { return this->status; }

	void SetStatus(GameStatus newStatus);
	bool IsWithinWorldView(const sf::Vector2f& point);
};