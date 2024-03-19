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
}

void Stage1::Reset()
{
	SetPosition({ 0,0 });
	SetOrigin(Origins::MC);
	stageNormal.SetPosition({ 0,0 });
	stageNormal.SetOrigin(Origins::MC);
}

void Stage1::Update(float dt)
{
	SpriteGo::Update(dt);
}

void Stage1::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	stageNormal.Draw(window);


}
