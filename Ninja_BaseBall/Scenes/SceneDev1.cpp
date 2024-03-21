#include "pch.h"
#include "SceneDev1.h"
#include "UiHUD.h"
#include "Tilemap.h"
#include "SpriteGo.h"
#include "TextGo.h"
#include "InputField.h"
#include "YellowBaseBall.h"
#include "Stage1.h"
#include "Player.h"
#include "Player2.h"
#include "YellowBaseBall.h"

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
    //cameraRect.setSize({ 320.f, 240 });
    //cameraRect.setOutlineColor(sf::Color::Cyan);
    //cameraRect.setOutlineThickness(2);
    //cameraRect.setFillColor(sf::Color::Transparent);

    worldView.setSize(windowSize);
    worldView.setCenter(0, 360);
    uiView.setSize(windowSize);
    uiView.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);

    stage = new Stage1();
    AddGameObject(stage);

    player = new Player("Player");
    player->SetPosition({ 100.f, 250.f });
    AddGameObject(player,World);

    yellowEnemy = new YellowBaseBall("YellowEnemy");
    yellowEnemy->SetPosition({ 300.f, 250.f });
    AddGameObject(yellowEnemy, World);

    //Player2* player2 = new Player2("Player2");
    //player->SetPosition({ 200.f, 250.f });
    //AddGameObject(player2);

    hud = new UiHUD();
    AddGameObject(hud, Ui);

    monster = new YellowBaseBall("Monster");
    AddGameObject(monster);

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

    if (player->GetPosition().x > xMax)
    {
        xMax = player->GetPosition().x;

    }
    float camCenter1 = stage->stageBound1_1.getGlobalBounds().left + (stage->stageBound1_1.getGlobalBounds().width / 2);
    float camCenter2 = stage->stageBound1_2.getGlobalBounds().left + (stage->stageBound1_2.getGlobalBounds().width / 2);
    float camCenter3 = stage->stageBound1_3.getGlobalBounds().left + (stage->stageBound1_3.getGlobalBounds().width / 2);
    float camCenter4 = stage->stageBound1_4.getGlobalBounds().left + (stage->stageBound1_4.getGlobalBounds().width / 2);

    if (!(stage->clearStage1_1) && xMax >= camCenter1)
    {
        xMax = camCenter1;
        player->SetPosition(Utils::MyMath::Clamp(player->GetPosition(), stage->stageBound1_1.getGlobalBounds()));
    }
    if (!(stage->clearStage1_2) && xMax >= camCenter2)
    {
        xMax = camCenter2;
        player->SetPosition(Utils::MyMath::Clamp(player->GetPosition(), stage->stageBound1_2.getGlobalBounds()));
    }
    if (!(stage->clearStage1_3) && xMax >= camCenter3)
    {
        xMax = camCenter3;
        player->SetPosition(Utils::MyMath::Clamp(player->GetPosition(), stage->stageBound1_3.getGlobalBounds()));
    }
    if (!(stage->clearStage1_4) && xMax >= camCenter4)
    {
        xMax = camCenter4;
        player->SetPosition(Utils::MyMath::Clamp(player->GetPosition(), stage->stageBound1_4.getGlobalBounds()));
    }

    sf::Vector2f worldViewCenter = worldView.getCenter();

    worldViewCenter.x = xMax;

    worldView.setCenter(worldViewCenter);

    stage->stageBack1.SetPosition({ worldViewCenter.x * 0.3f, 0.f });
    stage->stageBack2.SetPosition({ worldViewCenter.x * 0.3f + 1588.f, 0.f });


    if (InputManager::GetKeyDown(sf::Keyboard::Num1))
    {
        stage->clearStage1_1 = true;
    }
    else if (InputManager::GetKeyDown(sf::Keyboard::Num2))
    {
        stage->clearStage1_2 = true;
    }
    else if (InputManager::GetKeyDown(sf::Keyboard::Num3))
    {
        stage->clearStage1_3 = true;
    }
    else if (InputManager::GetKeyDown(sf::Keyboard::Num4))
    {
        stage->clearStage1_4 = true;
    }
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
    window.draw(cameraRect);
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
