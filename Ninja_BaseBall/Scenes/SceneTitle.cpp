#include "pch.h"
#include "SceneTitle.h"
#include "TextGo.h"
#include "SpriteGo.h"

SceneTitle::SceneTitle(SceneIDs id)
	: Scene(id)
{
	windowSize = (sf::Vector2f)FRAMEWORK.GetWindowSize();
}

void SceneTitle::Init()
{
	worldView.setSize(windowSize);
	worldView.setCenter(0, 0);
	uiView.setSize(windowSize);
	uiView.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);

	Scene::Init();
}

void SceneTitle::Release()
{
	Scene::Release();
}

void SceneTitle::Enter()
{
	Scene::Enter();
}

void SceneTitle::Exit()
{
	FRAMEWORK.SetTimeScale(1.f);

	Scene::Exit();
}

void SceneTitle::Update(float dt)
{
	Scene::Update(dt);

	if (InputManager::GetKeyDown(sf::Keyboard::Enter))
	{
		SCENE_MANAGER.ChangeScene(SceneIDs::SceneGame);
	}
}