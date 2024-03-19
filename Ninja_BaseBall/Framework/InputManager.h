#pragma once

enum class Axis
{
	Horizontal,
	Vertical,
};

struct AxisInfo
{
	Axis axis;

	std::list<sf::Keyboard::Key> positives;
	std::list<sf::Keyboard::Key> negatives;

	float sensitive;
	float value;
};

class InputManager
{
private:
	static std::map<Axis, AxisInfo> axisInfoMap;
	static std::list<sf::Keyboard::Key> downKeyList;
	static std::list<sf::Keyboard::Key> upKeyList;
	static std::list<sf::Keyboard::Key> ingKeyList;
	static sf::Vector2f mousePos;

public:
	static void Init();
	static void Update(float dt);
	static void UpdateEvent(const sf::Event& event);

	static void Clear();
	static bool GetKeyDown(sf::Keyboard::Key key);
	static bool GetKeyUp(sf::Keyboard::Key key);
	static bool GetKey(sf::Keyboard::Key key);

	static sf::Vector2f GetMousePos()
	{
		return mousePos;
	}

	static bool GetMouseButtonDown(sf::Mouse::Button mouse);
	static bool GetMouseButtonUp(sf::Mouse::Button mouse);
	static bool GetMouseButton(sf::Mouse::Button mouse);

	static float GetAxisRaw(Axis axis);
	static float GetAxis(Axis axis);

	static sf::Keyboard::Key MouseButtonToKey(sf::Mouse::Button button) { return (sf::Keyboard::Key)(button + sf::Keyboard::Key::KeyCount); };
	static sf::Mouse::Button KeytoMouseButton(sf::Keyboard::Key key) { return (sf::Mouse::Button)(key + sf::Keyboard::Key::KeyCount); };
};