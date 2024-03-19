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

	groundBound.setFillColor(sf::Color::Red);
	groundBound.setSize({ 1120.f,104.f });
	groundBound.setPosition({ 0.f, 136.f });
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

	window.draw(groundBound);

}
