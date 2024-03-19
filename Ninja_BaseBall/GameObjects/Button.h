#pragma once
#include "SpriteGo.h"

class TextGo;
class SceneAnimationTool;

class Button : public SpriteGo
{
public :
	enum class ButtonIdentifier
	{
		loadAtlas,
		play,
		stop,
		save
	};

protected:
	Button(const Button&) = delete;
	Button(Button&&) = delete;
	Button& operator=(const Button&) = delete;
	Button& operator=(Button&&) = delete;

	SceneAnimationTool* sceneAnimationTool = nullptr;
	ButtonIdentifier buttonIdentifier;

	std::wstring stringValue;

	sf::Text buttonText;
	sf::RectangleShape shape;

	sf::Color buttonColor = sf::Color(127, 127, 127, 255);
	sf::Color buttonColorDefault = sf::Color(127, 127, 127, 255);
	sf::Color buttonColorFocused = sf::Color(127, 127, 127, 200);
	sf::Color buttonColorPressed = sf::Color(127, 127, 127, 127);

	bool isButtonPressed;

public:
	Button(ButtonIdentifier identifier, const std::string& name = "button");
	~Button() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetOrigin(Origins preset)			  override;
	void SetOrigin(const sf::Vector2f& origin)override;

	void SetButton(sf::Vector2f size, sf::Vector2f position, sf::Color color, Origins origin);
	void SetButtonText(const sf::Font& font, const std::string& label, size_t labelSize, sf::Color labelColor, sf::Vector2f position, Origins origin);

	void SetButtonColor(sf::Color color);
	void SetButtonColorDefault(sf::Color color);
	void SetButtonColorFocused(sf::Color color);
	void SetButtonColorPressed(sf::Color color);

	void ExecuteButtonAction(ButtonIdentifier id);

	std::wstring OpenFileDialog(std::wstring& filePath);
	void SetStringValue(std::wstring& wstr) { stringValue = wstr; }

	void SaveSelectedAreasWithDialog();

	sf::FloatRect GetLocalBounds() override;
	sf::FloatRect GetGlobalBounds() override;
};

