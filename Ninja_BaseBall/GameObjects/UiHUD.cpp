#include "pch.h"
#include "UiHUD.h"
#include "SceneGame.h"

UiHUD::UiHUD(const std::string& name)
	: GameObject(name)
{
	windowSize = FRAMEWORK.GetWindowSize();
}

void UiHUD::Init()
{
	GameObject::Init();

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetScene(SceneIDs::SceneGame));
}

void UiHUD::Reset()
{
	GameObject::Reset();

}

void UiHUD::Update(float dt)
{
	GameObject::Update(dt);
}

void UiHUD::LateUpdate(float dt)
{
	GameObject::LateUpdate(dt);
}

void UiHUD::FixedUpdate(float dt)
{
	GameObject::FixedUpdate(dt);
}

void UiHUD::Draw(sf::RenderWindow& window)
{
	GameObject::Draw(window);
}