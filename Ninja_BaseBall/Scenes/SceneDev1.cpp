#include "pch.h"
#include "SceneDev1.h"
#include "UiHUD.h"
#include "Tilemap.h"
#include "SpriteGo.h"
#include "TextGo.h"
#include "InputField.h"
#include "Enemy.h"
#include "YellowBaseBall.h"
#include "Stage1.h"
#include "Player.h"
#include "Player2.h"


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
    worldView.setCenter(0, 360);
    uiView.setSize(windowSize);
    uiView.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);

    stage = new Stage1();
    AddGameObject(stage);

    player2 = new Player2("Player2");
    player2->SetPosition({ 400.f, 2500.f });
    AddGameObject(player2);

    player = new Player("Player");
    player->SetPosition({ 350.f, 500.f });
    AddGameObject(player, World);

    SpawnEnemy("YellowBaseBall", { 1400.f, 500.f });
    SpawnEnemy("YellowBaseBall", { 1800.f, 500.f });

    hud = new UiHUD();
    AddGameObject(hud, Ui);

    Scene::Init();
}

void SceneDev1::Release()
{
    Scene::Release();
    // for (Enemy* enemy : enemies) {
    //     delete enemy;
    // }
    //enemies.clear();
}

void SceneDev1::Reset()
{

}

void SceneDev1::Enter()
{
	Scene::Enter();
    status = GameStatus::Game;
    xMax = 500.f; //카메라 시작 지점

    player2->SetActive(false);
    player->SetActive(false);

}

void SceneDev1::Exit()
{
	FRAMEWORK.SetTimeScale(1.f);

    Scene::Exit();
}

void SceneDev1::Update(float dt)
{

    if (!(player->isImpacted))
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
    else
    {
        player->impactTimer -= dt;
        if (player->impactTimer <= 0.f)
        {
            player->isImpacted = false;
            player->impactTimer = 0.15f;
        }
    }

}

void SceneDev1::UpdateAwake(float dt)
{

}

void SceneDev1::UpdateGame(float dt)
{
    if (!isFighting && player->GetPosition().x > xMax)
    {
        xMax = player->GetPosition().x;
    }
    float camCenter1 = stage->stageBound1_1.getGlobalBounds().left + (stage->stageBound1_1.getGlobalBounds().width / 2);
    float camCenter2 = stage->stageBound1_2.getGlobalBounds().left + (stage->stageBound1_2.getGlobalBounds().width / 2);
    float camCenter3 = stage->stageBound1_3.getGlobalBounds().left + (stage->stageBound1_3.getGlobalBounds().width / 2);
    float camCenter4 = stage->stageBound1_4.getGlobalBounds().left + (stage->stageBound1_4.getGlobalBounds().width / 2);

    if (!(stage->clearStage1_1) && xMax >= camCenter1)
    {
        currStage = 1;
        isFighting = true;
        xMax = camCenter1;
        player->SetPosition(Utils::MyMath::Clamp(player->GetPosition(), stage->stageBound1_1.getGlobalBounds()));
        
        std::list <GameObject*> yellowBaseBallList;
        FindAll("YellowBaseBall", yellowBaseBallList);
        for (auto& yellowBaseBall : yellowBaseBallList)
        {
            yellowBaseBall->SetActive(true);
        }
    }
    if (!(stage->clearStage1_2) && xMax >= camCenter2)
    {
        currStage = 2;
        isFighting = true;
        xMax = camCenter2;
        player->SetPosition(Utils::MyMath::Clamp(player->GetPosition(), stage->stageBound1_2.getGlobalBounds()));
    }
    if (!(stage->clearStage1_3) && xMax >= camCenter3)
    {
        currStage = 3;
        isFighting = true;
        xMax = camCenter3;
        player->SetPosition(Utils::MyMath::Clamp(player->GetPosition(), stage->stageBound1_3.getGlobalBounds()));
    }
    if (!(stage->clearStage1_6) && xMax >= camCenter4)
    {
        currStage = 4;
        isFighting = true;
        xMax = camCenter4;
        player->SetPosition(Utils::MyMath::Clamp(player->GetPosition(), stage->stageBound1_4.getGlobalBounds()));
    }
    if (stage->clearStage1_4)
    {
        currStage = 5;
    }
    if (stage->clearStage1_5)
    {
        currStage = 6;
    }

    worldViewCenter = worldView.getCenter();

    worldViewCenter.x = xMax;
    worldViewCenter.y = player->GetPosition().y;

    if (cameraShakeOn)
    {
        CameraShake(dt);
    }

    worldView.setCenter(worldViewCenter);

    stage->stageBack1.SetPosition({ worldViewCenter.x * 0.3f - 200.f, 0.f });
    stage->stageBack2.SetPosition({ worldViewCenter.x * 0.3f + 1588.f, 0.f });
    
    if (InputManager::GetKeyDown(sf::Keyboard::Z))
    {
        player2->SetActive(true);
        player->SetActive(true);
    }

    if (InputManager::GetKeyDown(sf::Keyboard::Num0))
    {
        ClearStage();
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
        //FRAMEWORK.SetTimeScale(1.f);
        break;
    case GameStatus::GameOver:
        FRAMEWORK.SetTimeScale(0.f);
        break;
    case GameStatus::Pause:
        FRAMEWORK.SetTimeScale(0.f);
        break;
    }
}

void SceneDev1::SpawnEnemy(const std::string& type, const sf::Vector2f& position)
{
    if(type == "YellowBaseBall")
    {
        YellowBaseBall* enemy = new YellowBaseBall("YellowBaseBall");
        enemy->SetPosition(position);
        AddGameObject(enemy, World);
        enemies.push_back(enemy);
        enemy->SetActive(false);
    }
}

void SceneDev1::CameraShake(float dt)
{
    cameraShakeTime -= dt;

    int shakeTimeScaled = static_cast<int>(cameraShakeTime * 10);
    if (shakeTimeScaled % 2 == 0) // shakeTimeScaled가 짝수인 경우
    {
        // 카메라를 위로 흔들기
        worldViewCenter.y -= 0.4;
    }
    else
    {
        // 카메라를 아래로 흔들기
        worldViewCenter.y += 0.4;
    }

    if (cameraShakeTime <= 0.f)
    {
        cameraShakeOn = false;
        cameraShakeTime = 1.f;
    }
}

void SceneDev1::ClearStage()
{
    switch (currStage)
    {
        case 1:
            isFighting = false;
            stage->clearStage1_1 = true;
            break;
        case 2:
            isFighting = false;
            stage->clearStage1_2 = true;
            break;
        case 3:
            isFighting = false;
            stage->clearStage1_3 = true;
            break;
        case 4:
            isFighting = false;
            stage->clearStage1_4 = true;
            cameraShakeOn = true;
            break;
        case 5:
            isFighting = false;
            stage->clearStage1_5 = true;
            cameraShakeOn = true;
            break;
        case 6:
            isFighting = false;
            stage->clearStage1_6 = true;
            break;
        case 7:
            break;
    }

}
