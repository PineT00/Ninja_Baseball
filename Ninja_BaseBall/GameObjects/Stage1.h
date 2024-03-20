#pragma once
#include "SpriteGo.h"
class Stage1 :
    public SpriteGo
{
public:
	SpriteGo stageStart;
	SpriteGo stageNormal;
	SpriteGo stageNormal2;
	SpriteGo stageBroken;
	SpriteGo transition;
	SpriteGo stagePlane;
	SpriteGo stageBoss;

	sf::RectangleShape groundBound;
	sf::RectangleShape stageBound1_1;
	sf::RectangleShape stageBound1_2;
	sf::RectangleShape stageBound1_3;
	sf::RectangleShape stageBound1_4;

private:


public:
	Stage1(const std::string& name = "");
	~Stage1() = default;
	void Init() override;
	void Reset() override;
	void Update(float dt);
	void Draw(sf::RenderWindow& window) override;
};

