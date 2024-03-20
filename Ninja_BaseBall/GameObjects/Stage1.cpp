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
}

void Stage1::Reset()
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

	groundBound.setFillColor(sf::Color::Green);
	groundBound.setSize({ 1440.f,104.f });
	groundBound.setPosition({ 0.f, 136.f });

	stageBound1_1.setSize({ 320.f,240.f });
	stageBound1_1.setPosition({ 100.f, 0.f });
	stageBound1_1.setOutlineColor(sf::Color::Red);
	stageBound1_1.setOutlineThickness(1.f);
	stageBound1_1.setFillColor(sf::Color::Transparent);

	stageBound1_2.setFillColor(sf::Color::Red);
	stageBound1_2.setSize({ 320.f,240.f });
	stageBound1_2.setPosition({ 460.f, 0.f });
	stageBound1_2.setOutlineColor(sf::Color::Red);
	stageBound1_2.setOutlineThickness(1.f);
	stageBound1_2.setFillColor(sf::Color::Transparent);



}

void Stage1::Update(float dt)
{
	SpriteGo::Update(dt);
}

void Stage1::Draw(sf::RenderWindow& window)
{
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
	}

}
