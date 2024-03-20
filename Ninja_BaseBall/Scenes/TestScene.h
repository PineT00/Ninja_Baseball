#pragma once
#include "Scene.h"


class UiHUD;
class SpriteGo;
class TextGo;
class InputField;
class TestPlayer;
class YellowBaseBall;


class TestScene:public Scene
{
protected:
    TestScene(const TestScene&) = delete;
    TestScene(TestScene&&) = delete;
    TestScene& operator=(const TestScene&) = delete;
    TestScene& operator=(TestScene&&) = delete;

    sf::Vector2f windowSize;
    
    YellowBaseBall* yellowEnemy;
public:
    TestPlayer* testPlayer;
    TestScene(SceneIDs id);
    ~TestScene() override;

    void Init() override;
    
    void Release() override;
    virtual void Reset();
    void Enter() override;
    void Exit() override;
    void Update(float dt) override;
    void UpdateAwake(float dt);
    void UpdateGame(float dt);
    void UpdateGameover(float dt);
    void UpdatePause(float dt);
    void Draw(sf::RenderWindow& window);

    GameStatus GetStatus() const { return this->status; }
    void SetStatus(GameStatus newStatus);
};
