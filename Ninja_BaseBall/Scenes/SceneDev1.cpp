#include "pch.h"
#include "SceneDev1.h"
#include "UiHUD.h"
//#include "Tilemap.h"
//#include "SpriteGo.h"
//#include "TextGo.h"
//#include "InputField.h"
#include "Enemy.h"
#include "BaseBall.h"
#include "Stage1.h"
#include "Player.h"
#include "Player2.h"
#include "WindyPlane.h"
#include "Bat.h"
#include "Item.h"
#include "PickupItem.h"



SceneDev1::SceneDev1(SceneIDs id)
    : Scene(id)
{
    windowSize = (sf::Vector2f)FRAMEWORK.GetWindowSize();
}

sf::Vector2f SceneDev1::ClampByTileMap(const sf::Vector2f point) //현재 미사용!
{
    stageRect = stage->groundBound.getGlobalBounds();
    return Utils::MyMath::Clamp(point, stageRect);
}

void SceneDev1::Init()
{
    worldView.setSize(windowSize);
    worldView.setCenter(0, 360);
    uiView.setSize(windowSize);
    uiView.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);

    stage = new Stage1();
    AddGameObject(stage);

    // Player
    player = new Player("Player");
    player->SetPosition({ 350.f, 500.f });
    AddGameObject(player, World);

    SpawnEnemy("Stage1", { 1250.f, 500.f });
    SpawnEnemy("Stage2", { 1413.f, 500.f});
    SpawnEnemy("Stage3", { 2332.f, 500.f });
    SpawnEnemy("Stage4", { 3230.f, 500.f });
    SpawnEnemy("Stage5", { 3230.f, 500.f });
    SpawnEnemy("Stage6", { 3230.f, 500.f });
    SpawnEnemy("Stage7", { 3538.f, -1020.f });
    
    
    // Boss
    windyPlane = new WindyPlane();
    enemies.push_back(windyPlane);
    windyPlane->SetActive(false);
    AddGameObject(windyPlane);

    hud = new UiHUD();
    AddGameObject(hud, Ui);

    goldBatItem = new PickupItem(Item::Types::GoldBat, "goldbat");
    goldBatItem->Init();
    goldBatItem->SetTexture("graphics/5_Item/goldbat.bmp");
    goldBatItem->SetOrigin(Origins::MC);
    goldBatItem->SetActive(false);

    std::function<void(const std::string&)> pickupBatAction = std::bind(&Player::ItemPickup, player, std::placeholders::_1);
    goldBatItem->SetPickUpAction(pickupBatAction);
    AddGameObject(goldBatItem);

    Scene::Init();
}

void SceneDev1::Release()
{
    Scene::Release();
}

void SceneDev1::Reset()
{
    windyPlane->SetPosition({ stage->groundBoundBoss.getGlobalBounds().left + stage->groundBoundBoss.getGlobalBounds().width * 0.8f, stage->groundBoundBoss.getGlobalBounds().top + stage->groundBoundBoss.getGlobalBounds().height * 0.8f });
}

void SceneDev1::Enter()
{
	Scene::Enter();
    status = GameStatus::Game;
    Reset();

    xMax = 500.f; //카메라 시작 지점

    stageRect = stage->groundBound.getGlobalBounds(); //시작시 이동가능바닥
    windyPlane->SetPosition({ stage->groundBoundBoss.getGlobalBounds().left + stage->groundBoundBoss.getGlobalBounds().width * 0.8f, stage->groundBoundBoss.getGlobalBounds().top + stage->groundBoundBoss.getGlobalBounds().height * 0.8f });

    player->SetActive(false);

    camCenter1 = stage->stageBound1_1.getGlobalBounds().left + (stage->stageBound1_1.getGlobalBounds().width / 2);
    camCenter2 = stage->stageBound1_2.getGlobalBounds().left + (stage->stageBound1_2.getGlobalBounds().width / 2);
    camCenter3 = stage->stageBound1_3.getGlobalBounds().left + (stage->stageBound1_3.getGlobalBounds().width / 2);
    camCenter4 = stage->stageBound1_4.getGlobalBounds().left + (stage->stageBound1_4.getGlobalBounds().width / 2);
    camCenter7 = stage->stageBound1_7.getGlobalBounds().left + (stage->stageBound1_7.getGlobalBounds().width / 2);
    camCenter8 = stage->stageBound1_8.getGlobalBounds().left + (stage->stageBound1_8.getGlobalBounds().width / 2);
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
    //if (!player->GetActive() && player->life == 0)
    //{
    //    SetStatus(GameStatus::GameOver);
    //}
    if (InputManager::GetKeyDown(sf::Keyboard::Num9))
    {
        hud->GameOverCount();
        SetStatus(GameStatus::GameOver);
    }

    if (!windyPlane->GetAlive() && !goldBatItem->IsPicked())
    {
        goldBatItem->SetActive(true);
        goldBatItem->SetPosition(windyPlane->GetPosition());
    }

    if (!enterToBossFloor && player->currStatus == Player::Status::isIdleWalk)
    {
        player->SetPosition(Utils::MyMath::Clamp(player->GetPosition(), stageRect));
    }

    switch (currStage)
    {
        case 1:
            player->SetPosition(Utils::MyMath::Clamp(player->GetPosition(), stage->stageBound1_1.getGlobalBounds()));
            break;
        case 2:
            player->SetPosition(Utils::MyMath::Clamp(player->GetPosition(), stage->stageBound1_2.getGlobalBounds()));
            break;
        case 3:
            player->SetPosition(Utils::MyMath::Clamp(player->GetPosition(), stage->stageBound1_3.getGlobalBounds()));
            break;
        case 4:
            player->SetPosition(Utils::MyMath::Clamp(player->GetPosition(), stage->stageBound1_4.getGlobalBounds()));
            break;
        case 5:
            player->SetPosition(Utils::MyMath::Clamp(player->GetPosition(), stage->stageBound1_4.getGlobalBounds()));
            break;
        case 6:
            player->SetPosition(Utils::MyMath::Clamp(player->GetPosition(), stage->stageBound1_4.getGlobalBounds()));
            break;
        case 7:
            player->SetPosition(Utils::MyMath::Clamp(player->GetPosition(), stage->stageBound1_7.getGlobalBounds()));
            break;
        case 8:
            player->SetPosition(Utils::MyMath::Clamp(player->GetPosition(), stage->stageBound1_8.getGlobalBounds()));
            break;
        default:
            break;
    }


    if (!isFighting && player->GetPosition().x > xMax)
    {
        xMax = player->GetPosition().x;
    }


    if (!(stage->clearStage1_1))
    {
        if (xMax > camCenter1)
        {
            currStage = 1;
            FightOn();

            std::list <GameObject*> BaseBallList2;
            FindAll("BaseBallStage1", BaseBallList2);
            for (auto& BaseBall : BaseBallList2)
            {
                BaseBall->SetActive(true);
            }
        }
        else
        {
            int monsterNum = 2;
            std::list <GameObject*> BaseBallList2;
            FindAll("BaseBallStage1", BaseBallList2);
            for (auto& BaseBall : BaseBallList2)
            {
                if (BaseBall->GetActive() == 0)
                {
                    monsterNum -= 1;
                }
            }
            if (monsterNum == 0)
            {
                ClearStage();
            }

        }
    }
    else if (!(stage->clearStage1_2) )
    {
        if (xMax > camCenter2)
        {
            currStage = 2;
            FightOn();

            std::list <GameObject*> BaseBallList2;
            FindAll("BaseBallStage2", BaseBallList2);
            for (auto& BaseBall : BaseBallList2)
            {
                BaseBall->SetActive(true);
            }
        }
        else
        {
            int monsterNum = 2;
            std::list <GameObject*> BaseBallList2;
            FindAll("BaseBallStage2", BaseBallList2);
            for (auto& BaseBall : BaseBallList2)
            {
                if (BaseBall->GetActive() == 0)
                {
                    monsterNum -= 1;
                }
            }
            if (monsterNum == 0)
            {
                ClearStage();
            }
                
        }
        
    }
    else if (!(stage->clearStage1_3) )
    {
        if (xMax > camCenter3)
        {
            currStage = 3;
            FightOn();

            std::list <GameObject*> BaseBallList3;
            FindAll("BaseBallStage3", BaseBallList3);
            for (auto& BaseBall : BaseBallList3)
            {
                BaseBall->SetActive(true);
            }
        }
        else
        {
            int monsterNum = 2;
            std::list <GameObject*> BaseBallList3;
            FindAll("BaseBallStage3", BaseBallList3);
            for (auto& BaseBall : BaseBallList3)
            {
                if (BaseBall->GetActive() == 0)
                {
                    monsterNum -= 1;
                }
            }
            if (monsterNum == 0)
            {
                ClearStage();
            }
        } 
    }
    else if (!(stage->clearStage1_4) )
    {
        if (xMax > camCenter4)
        {
            currStage = 4;
            FightOn();

            std::list <GameObject*> BaseBallList4;
            FindAll("BaseBallStage4", BaseBallList4);
            for (auto& BaseBall : BaseBallList4)
            {
                BaseBall->SetActive(true);
            }
        }
        else
        {
            int monsterNum = 3;
            std::list <GameObject*> BaseBallList4;
            FindAll("BaseBallStage4", BaseBallList4);
            for (auto& BaseBall : BaseBallList4)
            {
                if (BaseBall->GetActive() == 0)
                {
                    monsterNum -= 1;
                }
            }
            if (monsterNum == 0)
            {
                ClearStage();
            }

        }

    }
    else if (stage->clearStage1_4 && !(stage->clearStage1_5) )
    {
        if (xMax > camCenter4)
        {
            currStage = 5;
            FightOn();

            std::list <GameObject*> BaseBallList5;
            FindAll("BaseBallStage5", BaseBallList5);
            for (auto& BaseBall : BaseBallList5)
            {
                BaseBall->SetActive(true);
            }
        }
        else
        {
            int monsterNum = 3;
            std::list <GameObject*> BaseBallList5;
            FindAll("BaseBallStage5", BaseBallList5);
            for (auto& BaseBall : BaseBallList5)
            {
                if (BaseBall->GetActive() == 0)
                {
                    monsterNum -= 1;
                }
            }
            if (monsterNum == 0)
            {
                ClearStage();
            }
        }

    }
    else if (stage->clearStage1_5 && !(stage->clearStage1_6) )
    {
        if (xMax > camCenter4)
        {
            currStage = 6;
            FightOn();

            std::list <GameObject*> BaseBallList6;
            FindAll("BaseBallStage6", BaseBallList6);
            for (auto& BaseBall : BaseBallList6)
            {
                BaseBall->SetActive(true);
            }
        }
        else
        {
            int monsterNum = 3;
            std::list <GameObject*> BaseBallList6;
            FindAll("BaseBallStage6", BaseBallList6);
            for (auto& BaseBall : BaseBallList6)
            {
                if (BaseBall->GetActive() == 0)
                {
                    monsterNum -= 1;
                }
            }
            if (monsterNum == 0)
            {
                ClearStage();
            }
        }

    }
    else if (stage->clearStage1_6 && !(stage->clearStage1_7) )
    {
        if (xMax > camCenter7)
        {
            currStage = 7;
            FightOn();

            std::list <GameObject*> BatList;
            FindAll("BaseBallStage7", BatList);
            for (auto& Bat : BatList)
            {
                Bat->SetActive(true);
            }
        }
        else
        {
            int monsterNum = 3;
            std::list <GameObject*> BatList;
            FindAll("BaseBallStage7", BatList);
            for (auto& Bat : BatList)
            {
                if (Bat->GetActive() == 0)
                {
                    monsterNum -= 1;
                }
            }
            if (monsterNum == 0)
            {
                ClearStage();
            }
        }

    }
    else if (stage->clearStage1_7 && !(stage->clearStage1_8))
    {
        if (xMax > camCenter8)
        {
            currStage = 8;
            windyPlane->SetActive(true);
            FightOn();
        }
    }

    worldViewCenter = worldView.getCenter();

    if (currStage == 8)
    {
        worldViewCenter.x = Utils::MyMath::Lerp(worldViewCenter.x, player->GetPosition().x, dt * 4.5f);
    }
    else
    {
        worldViewCenter.x = Utils::MyMath::Lerp(worldViewCenter.x, xMax, dt * 4.5f);
    }


    if (cameraShakeOn)
    {
        CameraShake(dt);
    }

    worldView.setCenter(worldViewCenter);

    stage->stageBack1.SetPosition({ worldViewCenter.x * 0.3f - 200.f, 0.f });
    stage->stageBack2.SetPosition({ worldViewCenter.x * 0.3f + 1588.f, 0.f });


    if (InputManager::GetKeyDown(sf::Keyboard::Z))
    {
        player->SetActive(true);
    }

    if (InputManager::GetKeyDown(sf::Keyboard::Num0))
    {
        ClearStage();
    }
}

void SceneDev1::UpdateGameover(float dt)
{
    //Bgm 추가

        

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
        FRAMEWORK.SetTimeScale(1.f);
        break;
    case GameStatus::Pause:
        FRAMEWORK.SetTimeScale(0.f);
        break;
    }
}

void SceneDev1::SpawnEnemy(const std::string& type, const sf::Vector2f& position)
{
    if(type == "Stage1")
    {
        BaseBall* BaseBall = BaseBall::Create(BaseBall::BaseBallColor::YELLOW,type);
        BaseBall->SetPosition(position);
        
        AddGameObject(BaseBall, World);
        enemies.push_back(BaseBall);
        BaseBall->SetActive(false);
    }
    if(type == "Stage1")
    {
        BaseBall* BaseBall = BaseBall::Create(BaseBall::BaseBallColor::YELLOW,type);
        BaseBall->SetPosition(position);
        
        AddGameObject(BaseBall, World);
        enemies.push_back(BaseBall);
        BaseBall->SetActive(false);
    }
    
    if(type == "Stage2")
    {
        BaseBall* BaseBall = BaseBall::Create(BaseBall::BaseBallColor::GREEN,type);
        BaseBall->SetPosition(position);
        
        AddGameObject(BaseBall, World);
        enemies.push_back(BaseBall);
        BaseBall->SetActive(false);
    }

    if(type == "Stage2")
    {
        BaseBall* BaseBall = BaseBall::Create(BaseBall::BaseBallColor::GREEN,type);
        BaseBall->SetPosition(position);
        
        AddGameObject(BaseBall, World);
        enemies.push_back(BaseBall);
        BaseBall->SetActive(false);
    }
    
    if(type == "Stage3")
    {
        BaseBall* BaseBall = BaseBall::Create(BaseBall::BaseBallColor::GREEN,type);
        BaseBall->SetPosition(position);
        
        AddGameObject(BaseBall, World);
        enemies.push_back(BaseBall);
        BaseBall->SetActive(false);
    }
    if(type == "Stage3")
    {
        BaseBall* BaseBall = BaseBall::Create(BaseBall::BaseBallColor::BLUE,type);
        BaseBall->SetPosition(position);
        
        AddGameObject(BaseBall, World);
        enemies.push_back(BaseBall);
        BaseBall->SetActive(false);
    }
    
    if(type == "Stage4")
    {
        BaseBall* BaseBall = BaseBall::Create(BaseBall::BaseBallColor::BLUE,type);
        BaseBall->SetPosition(position);
        
        AddGameObject(BaseBall, World);
        enemies.push_back(BaseBall);
        BaseBall->SetActive(false);
    }
    if(type == "Stage4")
    {
        BaseBall* BaseBall = BaseBall::Create(BaseBall::BaseBallColor::BLUE,type);
        BaseBall->SetPosition(position);
        
        AddGameObject(BaseBall, World);
        enemies.push_back(BaseBall);
        BaseBall->SetActive(false);
    }
    if(type == "Stage4")
    {
        BaseBall* BaseBall = BaseBall::Create(BaseBall::BaseBallColor::GREEN,type);
        BaseBall->SetPosition(position);
        
        AddGameObject(BaseBall, World);
        enemies.push_back(BaseBall);
        BaseBall->SetActive(false);
    }

    if(type == "Stage5")
    {
        BaseBall* BaseBall = BaseBall::Create(BaseBall::BaseBallColor::BLUE,type);
        BaseBall->SetPosition(position);
        
        AddGameObject(BaseBall, World);
        enemies.push_back(BaseBall);
        BaseBall->SetActive(false);
    }
    if(type == "Stage5")
    {
        BaseBall* BaseBall = BaseBall::Create(BaseBall::BaseBallColor::WHITE,type);
        BaseBall->SetPosition(position);
        
        AddGameObject(BaseBall, World);
        enemies.push_back(BaseBall);
        BaseBall->SetActive(false);
    }
    if(type == "Stage5")
    {
        BaseBall* BaseBall = BaseBall::Create(BaseBall::BaseBallColor::WHITE,type);
        BaseBall->SetPosition(position);
        
        AddGameObject(BaseBall, World);
        enemies.push_back(BaseBall);
        BaseBall->SetActive(false);
    }

    if(type == "Stage6")
    {
        BaseBall* BaseBall = BaseBall::Create(BaseBall::BaseBallColor::BLUE,type);
        BaseBall->SetPosition(position);
        
        AddGameObject(BaseBall, World);
        enemies.push_back(BaseBall);
        BaseBall->SetActive(false);
    }
    if(type == "Stage6")
    {
        BaseBall* BaseBall = BaseBall::Create(BaseBall::BaseBallColor::WHITE,type);
        BaseBall->SetPosition(position);
        
        AddGameObject(BaseBall, World);
        enemies.push_back(BaseBall);
        BaseBall->SetActive(false);
    }
    if(type == "Stage6")
    {
        BaseBall* BaseBall = BaseBall::Create(BaseBall::BaseBallColor::WHITE,type);
        BaseBall->SetPosition(position);
        
        AddGameObject(BaseBall, World);
        enemies.push_back(BaseBall);
        BaseBall->SetActive(false);
    }
    if(type == "Stage7")
    {
       Bat* Bat = Bat::Create(Bat::BatColor::YELLOW,type);
       Bat->SetPosition(position);
       AddGameObject(Bat, World);
       enemies.push_back(Bat);
       Bat->SetActive(false);
    }
    if(type == "Stage7")
    {
        Bat* Bat = Bat::Create(Bat::BatColor::YELLOW,type);
        Bat->SetPosition(position);
        AddGameObject(Bat, World);
        enemies.push_back(Bat);
        Bat->SetActive(false);
    }
    if(type == "Stage7")
    {
        Bat* Bat = Bat::Create(Bat::BatColor::YELLOW,type);
        Bat->SetPosition(position);
        AddGameObject(Bat, World);
        enemies.push_back(Bat);
        Bat->SetActive(false);
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

void SceneDev1::FightOn()
{
    std::cout << currStage << std::endl;

    isFighting = true;

    switch (currStage)
    {
        case 1:
            xMax = camCenter1;
            break;
        case 2:
            xMax = camCenter2;
            break;
        case 3:
            xMax = camCenter3;
            break;
        case 4:
            xMax = camCenter4;
            break;
        case 5:
            xMax = camCenter4;
            break;
        case 6:
            xMax = camCenter4;
            break;
        case 7:
            xMax = camCenter7;
            break;
        case 8:
            xMax = camCenter8;
            break;
    }
    
}

void SceneDev1::ClearStage()
{
    switch (currStage)
    {
        case 1:
            isFighting = false;
            stage->clearStage1_1 = true;
            currStage = 0;
            break;
        case 2:
            isFighting = false;
            stage->clearStage1_2 = true;
            currStage = 0;
            break;
        case 3:
            isFighting = false;
            stage->clearStage1_3 = true;
            currStage = 0;
            break;
        case 4:
            isFighting = false;
            stage->clearStage1_4 = true;
            cameraShakeOn = true;
            currStage = 0;
            break;
        case 5:
            isFighting = false;
            stage->clearStage1_5 = true;
            cameraShakeOn = true;
            currStage = 0;
            break;
        case 6:
            isFighting = false;
            stage->clearStage1_6 = true;
            currStage = 0;
            MoveToBoss();
            break;
        case 7:
            isFighting = false;
            stage->clearStage1_7 = true;
            currStage = 0;

            break;
        case 8:
            isFighting = false;
            stage->clearStage1_8 = true;
            break;
        default:
            break;
    }

}

void SceneDev1::MoveToBoss()
{
    enterToBossFloor = true;
    stageRect = stage->groundBoundBoss.getGlobalBounds();
    player->SetPosition({ player->GetPosition().x, -1090.f });
    worldViewCenter.y -= 1500.f;
    worldView.setCenter(worldViewCenter);
    enterToBossFloor = false;
}
