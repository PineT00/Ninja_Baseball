#pragma once
class ComboCommands
{

private:


public:
	InputManager::SFGM_COMBO* tackle = nullptr;

	std::vector<InputManager::SFGM_COMBO*> comboList;

	void SetCombo();
};

