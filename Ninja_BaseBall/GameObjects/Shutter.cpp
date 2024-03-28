#include "pch.h"
#include "Shutter.h"
#include "SceneDev1.h"
#include "Player.h"
#include "Enemy.h"

Shutter::Shutter(const std::string& name): SpriteGo(name), Scenedev1(nullptr), player(nullptr)
{
}

void Shutter::Init()
{
    SpriteGo::Init();
    animator.SetTarget(&sprite);
    shutterRect=GetGlobalBounds();
    
}

void Shutter::Reset()
{
    //SpriteGo::Reset();

    Scenedev1 = dynamic_cast<SceneDev1*>(SCENE_MANAGER.GetCurrentScene());
    player = dynamic_cast<Player*>(SCENE_MANAGER.GetCurrentScene()->FindGameObject("Player"));
    //enemy = dynamic_cast<Enemy*>(SCENE_MANAGER.GetCurrentScene()->FindGameObject(""));

    shutterRectShape.setSize(sf::Vector2f(shutterRect.width,shutterRect.height));
    shutterRectShape.setFillColor(sf::Color::Transparent);
    shutterRectShape.setOutlineColor(sf::Color::Red);

    
    SetState(ShutterStatus::OPEN);
    SetOrigin(Origins::BC);
    
}

void Shutter::Update(float dt)
{
    SpriteGo::Update(dt);
    animator.Update(dt);

    switch (shutterStatus)
    {
        case ShutterStatus::OPEN:
            UpdateOpen(dt);
            break;
        case ShutterStatus::CLOSE:
            UpdateClose(dt);
            break;
        case ShutterStatus::MOVE:
            UpdateMove(dt);
            break;
    }
}

void Shutter::Draw(sf::RenderWindow& window)
{
    SpriteGo::Draw(window);

    if(SCENE_MANAGER.GetDeveloperMode())
    {
        window.draw(shutterRectShape);
    }
}

void Shutter::SetState(ShutterStatus state)
{
    switch (state)
    {
        case ShutterStatus::OPEN:
            animator.Play("animations/Object/Shutter.csv");
            break;
        case ShutterStatus::CLOSE:
            animator.Play("animations/Object/Shutter.csv");
            break;
        case ShutterStatus::MOVE:
            animator.Play("animations/Object/Shutter.csv");
            break;
        case ShutterStatus::BREAK:
            animator.Play("animations/Object/Shutter.csv");
            break;
        default: break;
    }
}

void Shutter::UpdateOpen(float dt)
{
    if(Scenedev1->currStage==3)
    {
        shutterStatus = ShutterStatus::MOVE;
    }
}

void Shutter::UpdateClose(float dt)
{
   
}

void Shutter::UpdateMove(float dt)
{
    WaitTimer += dt;
    
    if (WaitTimer >= 5.f && this->GetPosition().y < 730)
    {
        sf::Vector2f currentPosition = this->GetPosition();
        currentPosition.y += 100 * dt;
        if (currentPosition.y > 730)
        {
            currentPosition.y = 730;
        }
        this->SetPosition(currentPosition);
        
        if (currentPosition.y == 730)
        {
            shutterStatus = ShutterStatus::CLOSE;
        }
    }
}

void Shutter::UpdateBreak(float dt)
{
    
}
