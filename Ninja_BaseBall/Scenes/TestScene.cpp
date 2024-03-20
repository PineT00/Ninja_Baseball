#include "pch.h"
#include "TestScene.h"
#include "TestPlayer.h"
#include "YellowBaseBall.h"



TestScene::TestScene(SceneIDs id):Scene(id)
{
    windowSize = static_cast<sf::Vector2f>(FRAMEWORK.GetWindowSize());
}

TestScene::~TestScene()
= default;

void TestScene::Init()
{
    worldView.setSize(windowSize);
    worldView.setCenter(0, 0);
    uiView.setSize(windowSize);
    uiView.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);

    testPlayer=new TestPlayer("TestPlayer");
    testPlayer->SetPosition({0.f,0.f});
    AddGameObject(testPlayer,World);

    yellowEnemy=new YellowBaseBall("YellowEnemy");
    yellowEnemy->SetPosition({100.f,100.f});
    AddGameObject(yellowEnemy, World);

    Scene::Init();
}

void TestScene::Release()
{
    Scene::Release();
}

void TestScene::Reset()
{
    
}

void TestScene::Enter()
{
    Scene::Enter();
    
    status = GameStatus::Game;
}

void TestScene::Exit()
{
    FRAMEWORK.SetTimeScale(1.f);
    
    Scene::Exit();
}

void TestScene::Update(float dt)
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

void TestScene::UpdateAwake(float dt)
{
}

void TestScene::UpdateGame(float dt)
{
    
    
}

void TestScene::UpdateGameover(float dt)
{
}

void TestScene::UpdatePause(float dt)
{
}

void TestScene::Draw(sf::RenderWindow& window)
{
    Scene::Draw(window);
    
}

void TestScene::SetStatus(GameStatus newStatus)
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

