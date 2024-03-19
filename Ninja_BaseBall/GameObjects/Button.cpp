#include "pch.h"
#include "Button.h"
#include "TextGo.h"

Button::Button(ButtonIdentifier identifier, const std::string& name)
	: SpriteGo(name), buttonIdentifier(identifier)
{
}

void Button::Init()
{	
	SpriteGo::Init();
}

void Button::Reset()
{
	//sceneUpgrade = dynamic_cast<SceneUpgrade*>(SCENE_MANAGER.GetCurrentScene());
}

void Button::Update(float dt)
{
	SpriteGo::Update(dt);
	SetButtonColor(buttonColor);

	if (InputManager::GetMouseButtonDown(sf::Mouse::Left))
	{
		if (GetGlobalBounds().contains(InputManager::GetMousePos()))
		{
			isButtonPressed = true;
			SetButtonColor(buttonColorPressed);
		}
	}
	else if (InputManager::GetMouseButtonUp(sf::Mouse::Left))
	{
		if (GetGlobalBounds().contains(InputManager::GetMousePos()))
		{
			if (isButtonPressed)
			{
				//ExecuteButtonAction();
				isButtonPressed = false;
			}
			SetButtonColor(buttonColor);
		}
	}
	else
	{
		if (GetGlobalBounds().contains(InputManager::GetMousePos()))
		{
			SetButtonColor(buttonColorFocused);
		}
		else
		{
			SetButtonColor(buttonColor); 
		}
	}
}

//void Button::ExecuteButtonAction()
//{
//	switch (buttonIdentifier)
//	{
//	case ButtonIdentifier::StartGame:
//		SaveGold();
//		SCENE_MANAGER.ChangeScene(SceneIDs::SceneGame);
//		break;
//	case ButtonIdentifier::PowerUp:
//		UpgradePowerLevel();
//		break;
//	case ButtonIdentifier::ExtraLife:
//		UpgradeExtraLifes();
//		break;
//	case ButtonIdentifier::Bomb:
//		UpgradeExtraBombs();
//		break;
//	}
//}

void Button::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	window.draw(shape);
	window.draw(buttonText);
}

void Button::SetOrigin(Origins preset)
{
	if (preset == Origins::CUSTOM)
	{
		preset == Origins::TL;
	}

	originPreset = preset;
	Utils::Origin::SetOrigin(shape, originPreset);
}

void Button::SetOrigin(const sf::Vector2f& origin)
{
	originPreset = Origins::CUSTOM;
	this->origin = origin;
	shape.setOrigin(this->origin);
}

void Button::SetButton(sf::Vector2f size, sf::Vector2f position, sf::Color color, Origins origin)
{
	shape.setSize(size);
	shape.setPosition(position);
	SetButtonColor(color);
	SetOrigin(origin);

}

void Button::SetButtonText(const sf::Font& font, const std::string& label, size_t labelSize, sf::Color labelColor, sf::Vector2f position, Origins origin)
{
	buttonText.setFont(font);
	buttonText.setString(label);
	buttonText.setCharacterSize(labelSize);
	buttonText.setFillColor(labelColor);
	buttonText.setPosition(position);
	Utils::Origin::SetOrigin(buttonText, origin);
}

void Button::SetButtonColor(sf::Color color)
{
	shape.setFillColor(color);
	SetButtonColorDefault(color);
	SetButtonColorFocused(color);
	SetButtonColorPressed(color);
}

void Button::SetButtonColorDefault(sf::Color color)
{
	color.a = 255.f;
	buttonColorDefault = color;
}

void Button::SetButtonColorFocused(sf::Color color)
{
	color.a = 200.f;
	buttonColorFocused = color;
}

void Button::SetButtonColorPressed(sf::Color color)
{
	color.a = 127.f;
	buttonColorPressed = color;
}

sf::FloatRect Button::GetLocalBounds()
{
	return shape.getLocalBounds();
}

sf::FloatRect Button::GetGlobalBounds()
{
	return shape.getGlobalBounds();
}
//
//void Button::UpgradePowerLevel()
//{
//	if (sceneUpgrade->GetCurrentGold() < 2000) return;
//	if (sceneUpgrade->GetExtraPowerLevel() >= 3) return;
//
//	sceneUpgrade->AddExtraPowerLevel(1);
//	sceneUpgrade->AddCurrentGold(-2000);
//}
//
//void Button::UpgradeExtraLifes()
//{
//	if (sceneUpgrade->GetCurrentGold() < 4000) return;
//	if (sceneUpgrade->GetExtraLifes() >= 7) return;
//
//	sceneUpgrade->AddExtraLifes(1);
//	sceneUpgrade->AddCurrentGold(-4000);
//}
//
//void Button::UpgradeExtraBombs()
//{
//	if (sceneUpgrade->GetCurrentGold() < 2000) return;
//	if (sceneUpgrade->GetExtraBombs() >= 7) return;
//
//	sceneUpgrade->SetExtraBombs(1);
//	sceneUpgrade->AddCurrentGold(-2000);
//}
//
//void Button::SaveGold()
//{
//	std::ofstream input;
//	input.open("gold.txt");
//	if (input.is_open())
//	{
//		input << sceneUpgrade->GetCurrentGold();
//	}
//}