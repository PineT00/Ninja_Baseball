#pragma once
#include "SpriteGo.h"
class Stage1 :
    public SpriteGo
{
public:
	SpriteGo stageStart;
	SpriteGo stageNormal;
	SpriteGo* stageBroken1;
	SpriteGo* stageBroken2;
	SpriteGo transition;
	SpriteGo stagePlane;
	SpriteGo stageBoss;

	SpriteGo stageBack1;
	SpriteGo stageBack2;
	SpriteGo stageBack3;


	sf::RectangleShape groundBound;
	sf::RectangleShape stageBound1_1;
	sf::RectangleShape stageBound1_2;
	sf::RectangleShape stageBound1_3;
	sf::RectangleShape stageBound1_4;


	bool clearStage1_1 = false;
	bool clearStage1_2 = false;
	bool clearStage1_3 = false;
	bool clearStage1_4 = false;

private:


public:
	Stage1(const std::string& name = "");
	~Stage1() = default;
	void Init() override;
	void Reset() override;
	void Update(float dt);
	void Draw(sf::RenderWindow& window) override;
};

