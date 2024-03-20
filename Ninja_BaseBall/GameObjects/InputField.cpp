#include "pch.h"
#include "InputField.h"
#include "SceneAnimationTool.h"

InputField::InputField(const std::string& name)
	: TextGo(name)
{
}

void InputField::Init()
{
	font = *FONT_MANAGER.GetResource("fonts/strikers1945.ttf");

	TextGo::Init();
	text.setFont(font);
	text.setCharacterSize(20);
	SetOrigin(Origins::BL);
}

void InputField::Reset()
{
	TextGo::Reset();

	sceneAnimationTool = dynamic_cast<SceneAnimationTool*>(SCENE_MANAGER.GetCurrentScene());

	shape.setSize(fieldSize);
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::White);
	shape.setOutlineThickness(2.f);
	Utils::Origin::SetOrigin(shape, Origins::ML);
}

void InputField::Update(float dt)
{
	TextGo::Update(dt);

	if (InputManager::GetMouseButtonUp(sf::Mouse::Left))
	{
		if (shape.getGlobalBounds().contains(sceneAnimationTool->ScreenToUi((sf::Vector2i)InputManager::GetMousePos())))
		{
			SetFocused(true);
		}
	}

	if (InputManager::GetKeyUp(sf::Keyboard::Enter) && isFocused)
	{
		isFocused = false;
	}

	if (isFocused)
	{
		SetText(GetText());
	}
}

void InputField::Draw(sf::RenderWindow& window)
{
	TextGo::Draw(window);
	window.draw(shape);
}

void InputField::SetPosition(float x, float y)
{
	TextGo::SetPosition(x, y);
	shape.setPosition({ x,y });
}

void InputField::SetPosition(const sf::Vector2f& pos)
{
	TextGo::SetPosition(pos);
	shape.setPosition(pos);
}

void InputField::SetFieldSize(const sf::Vector2f& size)
{
	fieldSize = size;
}