#include "pch.h"
#include "SceneGame.h"
#include "UiHUD.h"
#include "Tilemap.h"
#include "SpriteGo.h"
#include "TextGo.h"
#include "InputField.h"

SceneGame::SceneGame(SceneIDs id) 
    : Scene(id)
{
    windowSize = (sf::Vector2f)FRAMEWORK.GetWindowSize();
}

void SceneGame::Init()
{

    worldView.setSize(windowSize);
    worldView.setCenter(0, 0);
    uiView.setSize(windowSize);
    uiView.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);

    hud = new UiHUD();
    AddGameObject(hud, Layers::Ui);

    Scene::Init();
}

void SceneGame::Release()
{
    Scene::Release();
}

void SceneGame::Reset()
{
}

void SceneGame::Enter()
{
	Scene::Enter();
    status = GameStatus::Game;
}

void SceneGame::Exit()
{
	FRAMEWORK.SetTimeScale(1.f);

    Scene::Exit();
}

void SceneGame::Update(float dt)
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
    default:
        break;
    }
}

void SceneGame::UpdateAwake(float dt)
{

}

void SceneGame::UpdateGame(float dt)
{

}

void SceneGame::UpdateGameover(float dt)
{
    
}

void SceneGame::UpdatePause(float dt)
{

}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void SceneGame::SetStatus(GameStatus newStatus)
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
