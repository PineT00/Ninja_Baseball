#include "pch.h"
#include "SceneDev1.h"
#include "UiHUD.h"
#include "Tilemap.h"
#include "SpriteGo.h"
#include "TextGo.h"
#include "InputField.h"
#include "Stage1.h"
#include "Player.h"

SceneDev1::SceneDev1(SceneIDs id) 
    : Scene(id)
{
    windowSize = (sf::Vector2f)FRAMEWORK.GetWindowSize();
}

sf::Vector2f SceneDev1::ClampByTileMap(const sf::Vector2f point)
{
    sf::FloatRect rect = stage->groundBound.getGlobalBounds();

    return Utils::MyMath::Clamp(point, rect);
}

void SceneDev1::Init()
{
    worldView.setSize(windowSize);
    worldView.setCenter(0, 0);
    uiView.setSize(windowSize);
    uiView.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);


    stage = new Stage1();
    AddGameObject(stage);

    player = new Player();
    player->SetPosition({ 0.f, 300.f });
    AddGameObject(player);

    hud = new UiHUD();
    AddGameObject(hud, Ui);

    Scene::Init();
}

void SceneDev1::Release()
{
    Scene::Release();
}

void SceneDev1::Reset()
{
}

void SceneDev1::Enter()
{
	Scene::Enter();
    status = GameStatus::Game;
}

void SceneDev1::Exit()
{
	FRAMEWORK.SetTimeScale(1.f);

    Scene::Exit();
}

void SceneDev1::Update(float dt)
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

void SceneDev1::UpdateAwake(float dt)
{

}

void SceneDev1::UpdateGame(float dt)
{
    sf::Vector2f worldViewCenter = worldView.getCenter();

    worldViewCenter.x = player->GetPosition().x;
    worldViewCenter.y = player->GetPosition().y;


    worldView.setCenter(worldViewCenter);

    //backGround->SetPosition(worldViewCenter);

}

void SceneDev1::UpdateGameover(float dt)
{
    
}

void SceneDev1::UpdatePause(float dt)
{

}

void SceneDev1::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void SceneDev1::SetStatus(GameStatus newStatus)
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
