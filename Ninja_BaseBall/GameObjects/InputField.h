#pragma once
#include "TextGo.h"

class SceneAnimationTool;

class InputField : public TextGo
{
protected:
	InputField(const InputField&) = delete;
	InputField(InputField&&) = delete;
	InputField& operator=(const InputField&) = delete;
	InputField& operator=(InputField&&) = delete;

	SceneAnimationTool* sceneAnimationTool = nullptr;

	sf::Font font;
	sf::Vector2f fieldSize = {40, 20};
	sf::RectangleShape shape;

public:
	InputField(const std::string& name = "");
	~InputField() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetPosition(float x, float y);
	void SetPosition(const sf::Vector2f& pos);

	sf::Vector2f GetFieldSize() const { return fieldSize; }
	void SetFieldSize(const sf::Vector2f& size);

};