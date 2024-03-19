#include "pch.h"
#include "SceneAnimationTool.h"
#include "Button.h"
#include "SpriteGo.h"

SceneAnimationTool::SceneAnimationTool(SceneIDs id)
    : Scene(id)
{
    windowSize = (sf::Vector2f)FRAMEWORK.GetWindowSize();
}

void SceneAnimationTool::Init()
{
    font = *FONT_MANAGER.GetResource("fonts/strikers1945.ttf");

    worldView.setSize(windowSize);
    worldView.setCenter(0, 0);
    worldView.setViewport(sf::FloatRect(0.2f, 0.2f, 0.6f, 0.6f));
    uiView.setSize(windowSize);
    uiView.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);

    buttonLoadAtlas = new Button(Button::ButtonIdentifier::loadAtlas);
    buttonLoadAtlas->SetStringValue(atlasPath);
    buttonLoadAtlas->SetButton({ 200.f,80.f }, { windowSize.x / 2, windowSize.y / 2 }, sf::Color::Black, Origins::MC);
    buttonLoadAtlas->SetButtonText(font, "Button", 40.f, sf::Color::White, buttonLoadAtlas->GetPosition(), Origins::MC);

    spriteSheet = new SpriteGo("spritesheet");
    spriteSheet->Init();

    AddGameObject(buttonLoadAtlas, Layers::Ui);

    Scene::Init();
}

void SceneAnimationTool::Release()
{
    Scene::Release();
}

void SceneAnimationTool::Reset()
{
}

void SceneAnimationTool::Enter()
{
    Scene::Enter();
    status = GameStatus::Game;
}

void SceneAnimationTool::Exit()
{
    FRAMEWORK.SetTimeScale(1.f);

    Scene::Exit();
}

void SceneAnimationTool::Update(float dt)
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

void SceneAnimationTool::UpdateAwake(float dt)
{

}

void SceneAnimationTool::UpdateGame(float dt)
{
    if (atlasPath != L"" && !isAtlasPath)
    {
        isAtlasPath = true;
        spriteSheet->SetTexture(Utils::MyString::WideStringToString(atlasPath));
        spriteSheet->SetPosition({ 0,0 });
        spriteSheet->SetOrigin(Origins::MC);
        AddGameObject(spriteSheet);
    }
}

void SceneAnimationTool::UpdateGameover(float dt)
{

}

void SceneAnimationTool::UpdatePause(float dt)
{

}

void SceneAnimationTool::Draw(sf::RenderWindow& window)
{
    Scene::Draw(window);
}

void SceneAnimationTool::SetAtlasPath(const std::wstring& str)
{
    atlasPath = str;
}

void SceneAnimationTool::SetStatus(GameStatus newStatus)
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
