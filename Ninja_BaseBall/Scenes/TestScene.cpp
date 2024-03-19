#include "pch.h"
#include "TestScene.h"

#include "EnemySpawner.h"

TestScene::TestScene(SceneIDs id):Scene(id)
{
    windowSize = (sf::Vector2f)FRAMEWORK.GetWindowSize();
}

void TestScene::Init()
{
    worldView.setSize(windowSize);
    worldView.setCenter(0, 0);
    uiView.setSize(windowSize);
    uiView.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);

    EnemySpawner* enemySpawner = new EnemySpawner();
    enemySpawner->SpawnEnemy(Enemy::EnemyType::BASEBALL_YELLOW,sf::Vector2f(0,0));
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
    enemySpawner->Update(dt);
    
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
    enemySpawner->Draw(window);
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

