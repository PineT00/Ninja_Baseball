#include "pch.h"

std::map<Axis, AxisInfo> InputManager::axisInfoMap;

std::list<sf::Keyboard::Key> InputManager::downKeyList;
std::list<sf::Keyboard::Key> InputManager::upKeyList;
std::list<sf::Keyboard::Key> InputManager::ingKeyList;

sf::Vector2f InputManager::mousePos;

void InputManager::Init()
{
	// Horizontal
	AxisInfo infoHorizontal;
	infoHorizontal.axis = Axis::Horizontal;
	infoHorizontal.positives.push_back(sf::Keyboard::D);
	infoHorizontal.positives.push_back(sf::Keyboard::Right);
	infoHorizontal.negatives.push_back(sf::Keyboard::A);
	infoHorizontal.negatives.push_back(sf::Keyboard::Left);
	infoHorizontal.sensitive = 5.f;
	infoHorizontal.value = 0.f;

	axisInfoMap[infoHorizontal.axis] = infoHorizontal;

	// Vertical
	AxisInfo infoVertcal;
	infoVertcal.axis = Axis::Vertical;
	infoVertcal.positives.push_back(sf::Keyboard::S);
	infoVertcal.positives.push_back(sf::Keyboard::Down);
	infoVertcal.negatives.push_back(sf::Keyboard::W);
	infoVertcal.negatives.push_back(sf::Keyboard::Up);
	infoVertcal.sensitive = 5.f;
	infoVertcal.value = 0.f;

	axisInfoMap[infoVertcal.axis] = infoVertcal;
}

void InputManager::Update(float dt)
{
	mousePos = (sf::Vector2f)sf::Mouse::getPosition(FRAMEWORK.GetWindow());

	for (auto& pair : axisInfoMap)
	{
		AxisInfo& axisInfo = pair.second;

		float raw = GetAxisRaw(axisInfo.axis);
		float speed = raw;
		if (speed == 0.f && axisInfo.value != 0.f)
		{
			speed = axisInfo.value > 0.f ? -1.f : 1.f;
		}
		else
		{
			speed *= 2.f;
		}

		axisInfo.value += speed * axisInfo.sensitive * dt;
		axisInfo.value = Utils::MyMath::Clamp(axisInfo.value, -1.f, 1.f);

		bool over = (speed > 0) ? (axisInfo.value > 0.f) : (axisInfo.value < 0.f);
		if (raw == 0.f && over)
		{
			axisInfo.value = 0.f;
		}
	}
}

void InputManager::UpdateEvent(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		if (!GetKey(event.key.code))
		{
			downKeyList.push_back(event.key.code);
			ingKeyList.push_back(event.key.code);
		}
		break;

	case sf::Event::KeyReleased:
		ingKeyList.remove(event.key.code);
		upKeyList.push_back(event.key.code);
		break;

	case sf::Event::MouseButtonPressed:
		if (!GetMouseButton(event.mouseButton.button))
		{
			sf::Keyboard::Key button = MouseButtonToKey(event.mouseButton.button);
			downKeyList.push_back(button);
			ingKeyList.push_back(button);
		}
		break;
	case sf::Event::MouseButtonReleased:
		sf::Keyboard::Key button = MouseButtonToKey(event.mouseButton.button);
		ingKeyList.remove(button);
		upKeyList.push_back(button);
		break;
	}
}

void InputManager::Clear()
{
	downKeyList.clear();
	upKeyList.clear();
}

bool InputManager::GetKeyDown(sf::Keyboard::Key key)
{
	return std::find(downKeyList.begin(), downKeyList.end(), key) != downKeyList.end();
}

bool InputManager::GetKeyUp(sf::Keyboard::Key key)
{
	return std::find(upKeyList.begin(), upKeyList.end(), key) != upKeyList.end();
}

bool InputManager::GetKey(sf::Keyboard::Key key)
{
	return std::find(ingKeyList.begin(), ingKeyList.end(), key) != ingKeyList.end();
}

float InputManager::GetAxisRaw(Axis axis)
{
	auto findInfo = axisInfoMap.find(axis);
	if (findInfo == axisInfoMap.end())
	{
		return 0.f;
	}

	const auto& info = findInfo->second;
	auto it = ingKeyList.rbegin();
	while (it != ingKeyList.rend())
	{
		sf::Keyboard::Key key = *it;

		if (std::find(info.positives.begin(), info.positives.end(), key) != info.positives.end())
		{
			return 1.f;
		}

		if (std::find(info.negatives.begin(), info.negatives.end(), key) != info.negatives.end())
		{
			return -1.f;
		}

		++it;
	}

	return 0.0f;
}

float InputManager::GetAxis(Axis axis)
{
	auto findInfo = axisInfoMap.find(axis);
	if (findInfo == axisInfoMap.end())
	{
		return 0.f;
	}

	return findInfo->second.value;
}

bool InputManager::GetMouseButtonDown(sf::Mouse::Button mouse)
{
	return std::find(downKeyList.begin(), downKeyList.end(), MouseButtonToKey(mouse)) != downKeyList.end();
}

bool InputManager::GetMouseButtonUp(sf::Mouse::Button mouse)
{
	return std::find(upKeyList.begin(), upKeyList.end(), MouseButtonToKey(mouse)) != upKeyList.end();
}

bool InputManager::GetMouseButton(sf::Mouse::Button mouse)
{
	return std::find(ingKeyList.begin(), ingKeyList.end(), MouseButtonToKey(mouse)) != ingKeyList.end();
}