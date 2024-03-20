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
	stageNormal2.SetTexture("graphics/stage_normal.png");
	stageBroken.SetTexture("graphics/stage_broken.png");
	transition.SetTexture("graphics/stage_transition.png");
	stagePlane.SetTexture("graphics/stage_airplain.png");
	stageBoss.SetTexture("graphics/stage_airplain.png");

	stageBack1.SetTexture("graphics/background1-1.png");
	stageBack2.SetTexture("graphics/background1-2.png");
	stageBack3.SetTexture("graphics/background1-3.png");

}

void Stage1::Reset()
{
	//스테이지 스프라이트
	{
		SetPosition({ 0,0 });
		SetOrigin(Origins::TL);
		stageNormal.SetPosition({ 800,0 });
		stageNormal.SetOrigin(Origins::TL);
		stageNormal2.SetPosition({ 1120,0 });
		stageNormal2.SetOrigin(Origins::TL);
		stageBroken.SetPosition({ 1120,0 });
		stageBroken.SetOrigin(Origins::TL);
		transition.SetPosition({ 1120,-240 });
		transition.SetOrigin(Origins::TL);
		stagePlane.SetPosition({ 1120,-480 });
		stagePlane.SetOrigin(Origins::TL);
		stageBoss.SetPosition({ 1760,-480 });
		stageBoss.SetOrigin(Origins::TL);
	}

	//충돌영역
	{
		groundBound.setFillColor(sf::Color::Green);
		groundBound.setSize({ 1440.f,100.f });
		groundBound.setPosition({ 0.f, 138.f });

		stageBound1_1.setSize({ 320.f,240.f });
		stageBound1_1.setPosition({ 100.f, 0.f });
		stageBound1_1.setOutlineColor(sf::Color::Blue);
		stageBound1_1.setOutlineThickness(1.f);
		stageBound1_1.setFillColor(sf::Color::Transparent);

		stageBound1_2.setFillColor(sf::Color::Red);
		stageBound1_2.setSize({ 320.f,240.f });
		stageBound1_2.setPosition({ 140.f, 0.f });
		stageBound1_2.setOutlineColor(sf::Color::Red);
		stageBound1_2.setOutlineThickness(3.f);
		stageBound1_2.setFillColor(sf::Color::Transparent);

		stageBound1_3.setFillColor(sf::Color::Red);
		stageBound1_3.setSize({ 320.f,240.f });
		stageBound1_3.setPosition({ 460.f, 0.f });
		stageBound1_3.setOutlineColor(sf::Color::Red);
		stageBound1_3.setOutlineThickness(1.f);
		stageBound1_3.setFillColor(sf::Color::Transparent);

		stageBound1_4.setFillColor(sf::Color::Red);
		stageBound1_4.setSize({ 320.f,240.f });
		stageBound1_4.setPosition({ 780.f, 0.f });
		stageBound1_4.setOutlineColor(sf::Color::Red);
		stageBound1_4.setOutlineThickness(1.f);
		stageBound1_4.setFillColor(sf::Color::Transparent);
	}

	stageBack1.SetPosition({ 0.f, 0.f });
	stageBack2.SetPosition({ 566.f, 0.f });



}

void Stage1::Update(float dt)
{
	SpriteGo::Update(dt);
}

void Stage1::Draw(sf::RenderWindow& window)
{
	stageBack1.Draw(window);
	stageBack2.Draw(window);

	SpriteGo::Draw(window);
	stageNormal.Draw(window);
	stageNormal2.Draw(window);
	//stageBroken.Draw(window);
	transition.Draw(window);
	stagePlane.Draw(window);
	stageBoss.Draw(window);



	if (SCENE_MANAGER.GetDeveloperMode())
	{
		window.draw(groundBound);
		window.draw(stageBound1_1);
		window.draw(stageBound1_2);
		window.draw(stageBound1_3);
		window.draw(stageBound1_4);
	}


}
