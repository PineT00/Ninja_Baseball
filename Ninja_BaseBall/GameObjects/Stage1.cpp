#include "pch.h"
#include "Stage1.h"

Stage1::Stage1(const std::string& name)
	:SpriteGo(name)
{
}

void Stage1::Init()
{
	SpriteGo::Init();

	SetTexture("graphics/stage_start.png");
	stageNormal.SetTexture("graphics/stage_normal.png");
	stageBroken1.SetTexture("graphics/stage_broken1.png");
	stageBroken2.SetTexture("graphics/stage_broken2.png");
	transition.SetTexture("graphics/stage_transition.png");
	stagePlane.SetTexture("graphics/stage_airplain.png");
	stageBack1.SetTexture("graphics/background1-1.png");
	stageBack2.SetTexture("graphics/background1-2.png");

	stageBack3_1.SetTexture("graphics/background1-3.png"); //비행기
	stageBack3_2.SetTexture("graphics/background1-3.png"); //비행기
	stageBack3_3.SetTexture("graphics/background1-3.png"); //비행기

	//스테이지 스프라이트
	{
		SetPosition({ 0,0 });
		SetOrigin(Origins::TL);
		stageNormal.SetPosition({ 2400,0 });
		stageNormal.SetOrigin(Origins::TL);
		stageBroken1.SetPosition({ 2400,0 });
		stageBroken1.SetOrigin(Origins::TL);
		stageBroken2.SetPosition({ 2400,0 });
		stageBroken2.SetOrigin(Origins::TL);
		transition.SetPosition({ 2400,-760 });
		transition.SetOrigin(Origins::TL);
		stagePlane.SetPosition({ 2400,-1520 });
		stagePlane.SetOrigin(Origins::TL);


	}


	//충돌영역
	{
		groundBound.setFillColor(sf::Color::Green);
		groundBound.setSize({ 3300.f,300.f });
		groundBound.setPosition({ 0.f, 400.f });

		groundBoundBoss.setFillColor(sf::Color::Green);
		groundBoundBoss.setSize({ 1920.f,270.f });
		groundBoundBoss.setPosition({ 2400.f,-1090.f });

		stageBound1_1.setSize({ 960.f,720.f });
		stageBound1_1.setPosition({ 300.f, 0.f });
		stageBound1_1.setOutlineColor(sf::Color::Blue);
		stageBound1_1.setOutlineThickness(1.f);
		stageBound1_1.setFillColor(sf::Color::Transparent);

		stageBound1_2.setFillColor(sf::Color::Red);
		stageBound1_2.setSize({ 960.f,720.f });
		stageBound1_2.setPosition({ 420.f, 0.f });
		stageBound1_2.setOutlineColor(sf::Color::Red);
		stageBound1_2.setOutlineThickness(3.f);
		stageBound1_2.setFillColor(sf::Color::Transparent);

		stageBound1_3.setFillColor(sf::Color::Red);
		stageBound1_3.setSize({ 700.f,720.f });
		stageBound1_3.setPosition({ 1380.f, 0.f });
		stageBound1_3.setOutlineColor(sf::Color::Red);
		stageBound1_3.setOutlineThickness(1.f);
		stageBound1_3.setFillColor(sf::Color::Transparent);
		

		stageBound1_4.setFillColor(sf::Color::Red);
		stageBound1_4.setSize({ 960.f,720.f });
		stageBound1_4.setPosition({ 2400.f, 0.f });
		stageBound1_4.setOutlineColor(sf::Color::Red);
		stageBound1_4.setOutlineThickness(1.f);
		stageBound1_4.setFillColor(sf::Color::Transparent);

		stageBound1_7.setFillColor(sf::Color::Red);
		stageBound1_7.setSize({ 960.f,720.f });
		stageBound1_7.setPosition({ 2400.f, -1440.f });
		stageBound1_7.setOutlineColor(sf::Color::Red);
		stageBound1_7.setOutlineThickness(1.f);
		stageBound1_7.setFillColor(sf::Color::Transparent);

		stageBound1_8.setFillColor(sf::Color::Red);
		stageBound1_8.setSize({ 1920.f,720.f });
		stageBound1_8.setPosition({ 2400.f, -1440.f });
		stageBound1_8.setOutlineColor(sf::Color::Red);
		stageBound1_8.setOutlineThickness(1.f);
		stageBound1_8.setFillColor(sf::Color::Transparent);

	}
}

void Stage1::Reset()
{
	backPosition3_1 = { 2400.f,-1520.f };
	backPosition3_2 = { 3900.f,-1520.f };
	backPosition3_3 = { 5400.f,-1520.f };

	stageBack3_1.SetPosition(backPosition3_1);
	stageBack3_1.SetOrigin(Origins::TL);
	stageBack3_2.SetPosition(backPosition3_2); 
	stageBack3_2.SetOrigin(Origins::TL);
	stageBack3_2.SetPosition(backPosition3_3);
	stageBack3_2.SetOrigin(Origins::TL);


	clearStage1_1 = false;
	clearStage1_2 = false;
	clearStage1_3 = false;
	clearStage1_4 = false;
	clearStage1_5 = false;
	clearStage1_6 = false;
	clearStage1_7 = false;
	clearStage1_8 = false;


}

void Stage1::Update(float dt)
{
	SpriteGo::Update(dt);

	backPosition3_1.x -= backSpeed * dt;
	backPosition3_2.x -= backSpeed * dt;
	backPosition3_3.x -= backSpeed * dt;

	if (backPosition3_1.x < 900.f)
	{
		backPosition3_1.x = 5400.f;
	}
	if (backPosition3_2.x < 900.f)
	{
		backPosition3_2.x = 5400.f;
	}
	if (backPosition3_3.x < 900.f)
	{
		backPosition3_3.x = 5400.f;
	}

	stageBack3_1.SetPosition(backPosition3_1);
	stageBack3_2.SetPosition(backPosition3_2);
	stageBack3_3.SetPosition(backPosition3_3);
}

void Stage1::Draw(sf::RenderWindow& window)
{
	stageBack1.Draw(window);
	stageBack2.Draw(window);
	stageBack3_1.Draw(window);
	stageBack3_2.Draw(window);
	stageBack3_3.Draw(window);

	SpriteGo::Draw(window);

	transition.Draw(window);
	stagePlane.Draw(window);

	if (!clearStage1_4)
	{
		stageNormal.Draw(window);
	}
	else if (clearStage1_4 && !clearStage1_5)
	{
		stageBroken1.Draw(window);
	}
	else
	{
		stageBroken2.Draw(window);
	}



	if (SCENE_MANAGER.GetDeveloperMode())
	{
		window.draw(groundBound);
		window.draw(groundBoundBoss);
		window.draw(stageBound1_1);
		window.draw(stageBound1_2);
		window.draw(stageBound1_3);
		window.draw(stageBound1_4);
		window.draw(stageBound1_7);
		window.draw(stageBound1_8);
	}


}
