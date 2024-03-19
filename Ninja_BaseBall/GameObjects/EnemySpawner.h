#pragma once
#include "Enemy.h"

class EnemySpawner
{
public:
    EnemySpawner();
    ~EnemySpawner();

    std::vector<std::unique_ptr<Enemy>> enemies;

    void SpawnEnemy(Enemy::EnemyType type, sf::Vector2f position);
    void Update(float dt);
    void Draw(sf::RenderWindow& window);
};
