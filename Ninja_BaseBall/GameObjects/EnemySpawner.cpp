#include "pch.h"
#include "EnemySpawner.h"

EnemySpawner::EnemySpawner()
{
}

EnemySpawner::~EnemySpawner()
{
}

void EnemySpawner::SpawnEnemy(Enemy::EnemyType type, sf::Vector2f position)
{
    auto enemy = Enemy::Create(type);
    enemy->SetPosition(position);
    enemies.push_back(std::unique_ptr<Enemy>(enemy));
}

void EnemySpawner::Update(float dt)
{
    for(auto& enemy : enemies)
    {
        enemy->Update(dt);
    }
}

void EnemySpawner::Draw(sf::RenderWindow& window)
{
    for(auto& enemy : enemies)
    {
        enemy->Draw(window);
    }
}
