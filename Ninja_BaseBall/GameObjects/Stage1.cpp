#include "pch.h"
#include "Stage1.h"

Stage1::Stage1(const std::string& name)
	:SpriteGo(name)
{
}

void Stage1::Init()
{
	SpriteGo::Init();
	stageStart.SetTexture("graphics/background1.png");
}

void Stage1::Reset()
{
	stageStart.SetPosition({ 0,0 });
	stageStart.SetOrigin(originPreset);
}

void Stage1::Update(float dt)
{
	SpriteGo::Update(dt);
}