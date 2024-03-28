#include "pch.h"
#include "Shutter.h"
#include "SceneDev1.h"
#include "Player.h"
#include "Enemy.h"

Shutter::Shutter(const std::string& name) : SpriteGo(name), Scenedev1(nullptr), player(nullptr)
{
}

void Shutter::Init()
{
    SpriteGo::Init();
    animator.SetTarget(&sprite);
    shutterRect = GetGlobalBounds();

}

void Shutter::Reset()
{
    //SpriteGo::Reset();
    isBroken = false;
    WaitTimer = 0;

    Scenedev1 = dynamic_cast<SceneDev1*>(SCENE_MANAGER.GetCurrentScene());
    player = dynamic_cast<Player*>(SCENE_MANAGER.GetCurrentScene()->FindGameObject("Player"));

    shutterRectShape.setSize(sf::Vector2f(shutterRect.width, shutterRect.height));
    shutterRectShape.setFillColor(sf::Color::Transparent);
    shutterRectShape.setOutlineColor(sf::Color::Red);


    SetState(ShutterStatus::OPEN);
    SetOrigin(Origins::BC);

}

void Shutter::Update(float dt)
{
    SpriteGo::Update(dt);
    animator.Update(dt);

    switch (currStatus)
    {
        case ShutterStatus::OPEN:
            UpdateOpen(dt);
            break;
        case ShutterStatus::CLOSED:
            UpdateClosed(dt);
            break;
        case ShutterStatus::DOWN:
            UpdateDown(dt);
            break;
        case ShutterStatus::BREAK:
            UpdateBreak(dt);
            break;
    }
}

void Shutter::Draw(sf::RenderWindow& window)
{
    if (GetActive())
    {
        SpriteGo::Draw(window);
    }

    if (SCENE_MANAGER.GetDeveloperMode())
    {
        window.draw(shutterRectShape);
    }
}

void Shutter::SetState(ShutterStatus state)
{
    currStatus = state;
    switch (currStatus)
    {
        case ShutterStatus::OPEN:
            animator.Play("animations/Object/Shutter.csv");
            break;
        case ShutterStatus::CLOSED:
            animator.Play("animations/Object/Shutter.csv");
            break;
        case ShutterStatus::DOWN:
            animator.Play("animations/Object/Shutter.csv");
            break;
        case ShutterStatus::BREAK:
            animator.Play("animations/Object/ShutterBreak.csv");
            break;
        default: break;
    }
}

void Shutter::UpdateOpen(float dt)
{
    if (Scenedev1->currStage == 3)
    {
        currStatus = ShutterStatus::DOWN;
    }
}

void Shutter::UpdateClosed(float dt)
{
    if (player->GetPosition().x > 2000.f && player->GetPosition().y > 0.f)
    {
        if (InputManager::GetKeyDown(sf::Keyboard::Q))
        {
            SetState(ShutterStatus::BREAK);
        }
    }
    
}

void Shutter::UpdateDown(float dt)
{
    WaitTimer += dt;

    if (WaitTimer >= 3.f && this->GetPosition().y < 730)
    {
        sf::Vector2f currentPosition = this->GetPosition();
        currentPosition.y += 150 * dt;
        if (currentPosition.y > 730)
        {
            currentPosition.y = 730;
        }
        this->SetPosition(currentPosition);

        if (currentPosition.y == 730)
        {
            SetState(ShutterStatus::CLOSED);
        }
    }
}

void Shutter::UpdateBreak(float dt)
{
    isBroken = true;
}
