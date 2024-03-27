#pragma once
#include "SpriteGo.h"
class Stage1 :
    public SpriteGo
{
public:
	SpriteGo stageStart;
	SpriteGo stageNormal;
	SpriteGo stageBroken1;
	SpriteGo stageBroken2;
	SpriteGo transition;
	SpriteGo stagePlane;
	SpriteGo stageBoss;

	SpriteGo stageBack1;
	SpriteGo stageBack2;
	SpriteGo stageBack3;


	sf::RectangleShape groundBound;
	sf::RectangleShape groundBoundBoss;
	sf::RectangleShape stageBound1_1; //야구공(기본)
	sf::RectangleShape stageBound1_2; //야구공(발차기)
	sf::RectangleShape stageBound1_3; //야구공(덤블링)
	sf::RectangleShape stageBound1_4; //글러브
	//sf::RectangleShape stageBound1_5; //부서진 스테이지1 //글러브
	//sf::RectangleShape stageBound1_6; //부서진 스테이지2 //글러브, 야구공 (원본중간보스)
	sf::RectangleShape stageBound1_7; //비행기 스테이지 //배트
	sf::RectangleShape stageBound1_8; //비행기 보스 스테이지

	bool clearStage1_1 = false;
	bool clearStage1_2 = false;
	bool clearStage1_3 = false;
	bool clearStage1_4 = false;
	bool clearStage1_5 = false; // 부서진 스테이지1
	bool clearStage1_6 = false; // 부서진 스테이지2
	bool clearStage1_7 = false; // 비행기 스테이지
	bool clearStage1_8 = false; // 비행기 보스 스테이지

private:


public:
	Stage1(const std::string& name = "");
	~Stage1() = default;
	void Init() override;
	void Reset() override;
	void Update(float dt);
	void Draw(sf::RenderWindow& window) override;
};

