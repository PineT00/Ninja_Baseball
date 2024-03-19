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

private:


public:
	Stage1(const std::string& name = "");
	~Stage1() = default;
	void Init() override;
	void Reset() override;
	void Update(float dt);
	void Draw(sf::RenderWindow& window) override;
};

