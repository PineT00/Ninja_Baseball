#include "pch.h"
#include "ComboCommands.h"

void ComboCommands::SetCombo()
{
	tackle = new InputManager::SFGM_COMBO;
	comboList.push_back(tackle);
	tackle->push_back(std::make_pair(sf::Keyboard::S, InputManager::KEY_STATE::DOWN));
	tackle->push_back(std::make_pair(sf::Keyboard::S, InputManager::KEY_STATE::UP));
	tackle->push_back(std::make_pair(sf::Keyboard::X, InputManager::KEY_STATE::DOWN));
	tackle->push_back(std::make_pair(sf::Keyboard::X, InputManager::KEY_STATE::UP));
}
