#include "pch.h"
#include "TestPlayer.h"
#include "TestScene.h"
#include "YellowBaseBall.h"


TestPlayer::TestPlayer(const std::string& name): SpriteGo(name), testScene(nullptr), enemy(nullptr)
{
}

void TestPlayer::Init()
{
    SpriteGo::Init();
    playerAnimator.SetTarget(&sprite);
    //textureId = "Player";
}

void TestPlayer::Reset()
{
    //SpriteGo::Reset();
    testScene = dynamic_cast<TestScene*>(SCENE_MANAGER.GetCurrentScene());
    //enemy = dynamic_cast<YellowBaseBall*>(SCENE_MANAGER.GetCurrentScene()->FindGameObject("YellowBaseBall"));
    playerAnimator.Play("animations/Player_Idle.csv",false);
    SetPosition({0.f,0.f});
    //SetOrigin(Origins::BC);
}

void TestPlayer::Update(float dt)
{
    SpriteGo::Update(dt);
    playerAnimator.Update(dt);
    if(InputManager::GetKeyDown(sf::Keyboard::Space))
    {
        Attack();
    }
    if(InputManager::GetAxisRaw(Axis::Horizontal) != 0)
    {
        position.x+=InputManager::GetAxisRaw(Axis::Horizontal) * speed * dt;
    }
    if(InputManager::GetAxisRaw(Axis::Vertical) != 0)
    {
        position.y+=InputManager::GetAxisRaw(Axis::Vertical) * speed * dt;
    }
    if(testScene != nullptr)
    {
        position = testScene->ClampByTileMap(position);
    }
    
    sprite.setPosition(position);
}

void TestPlayer::OnDamage(int damage)
{
    health -= damage;
}

void TestPlayer::LateUpdate(float dt)
{
    SpriteGo::LateUpdate(dt);
}

void TestPlayer::FixedUpdate(float dt)
{
    SpriteGo::FixedUpdate(dt);
}

void TestPlayer::Attack()
{
    //enemy->OnDamage(damage);
}

void TestPlayer::Draw(sf::RenderWindow& window)
{
    //SpriteGo::Draw(window);
    window.draw(sprite);
}



