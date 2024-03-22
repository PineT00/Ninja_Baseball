#include "pch.h"
#include "SceneDevBoss.h"

SceneDevBoss::SceneDevBoss(SceneIDs id)
	: Scene(id)
{
    windowSize = (sf::Vector2f)FRAMEWORK.GetWindowSize();
}

void SceneDevBoss::Init()
{
    // 보스 추가

	Scene::Init();
}

void SceneDevBoss::Release()
{
}

void SceneDevBoss::Reset()
{
}

void SceneDevBoss::Enter()
{
	Scene::Enter();
    status = GameStatus::Game;
}

void SceneDevBoss::Exit()
{

	FRAMEWORK.SetTimeScale(1.f);
	Scene::Exit();
}

void SceneDevBoss::Update(float dt)
{
    Scene::Update(dt);
    SetStatus(status);

    switch (status)
    {
    case GameStatus::Awake:
        UpdateAwake(dt);
        break;
    case GameStatus::Game:
        UpdateGame(dt);
        break;
    case GameStatus::GameOver:
        UpdateGameover(dt);
        break;
    case GameStatus::Pause:
        UpdatePause(dt);
        break;
    }
}

void SceneDevBoss::UpdateAwake(float dt)
{
}

void SceneDevBoss::UpdateGame(float dt)
{
}

void SceneDevBoss::UpdateGameover(float dt)
{
}

void SceneDevBoss::UpdatePause(float dt)
{
}

void SceneDevBoss::Draw(sf::RenderWindow& window)
{
    Scene::Draw(window);
}

void SceneDevBoss::SetStatus(GameStatus newStatus)
{
    GameStatus prevStatus = status;

    status = newStatus;

    switch (status)
    {
    case GameStatus::Awake:
        FRAMEWORK.SetTimeScale(0.f);
        break;
    case GameStatus::Game:
        FRAMEWORK.SetTimeScale(1.f);
        break;
    case GameStatus::GameOver:
        FRAMEWORK.SetTimeScale(0.f);
        break;
    case GameStatus::Pause:
        FRAMEWORK.SetTimeScale(0.f);
        break;
    }
}
