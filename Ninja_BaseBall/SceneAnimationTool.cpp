#include "pch.h"
#include "SceneAnimationTool.h"
#include "Button.h"
#include "SpriteGo.h"
#include "InputField.h"
#include "TextGo.h"

SceneAnimationTool::SceneAnimationTool(SceneIDs id)
    : Scene(id)
{
    windowSize = (sf::Vector2f)FRAMEWORK.GetWindowSize();
}

void SceneAnimationTool::Init()
{
    font = *FONT_MANAGER.GetResource("fonts/strikers1945.ttf");

    worldView.setSize(windowSize);
    worldView.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);
    worldView.setViewport(sf::FloatRect(0.2f, 0.2f, 0.6f, 0.6f));
    uiView.setSize(windowSize);
    uiView.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);

    buttonLoadAtlas = new Button(Button::ButtonIdentifier::loadAtlas);
    buttonLoadAtlas->SetStringValue(atlasPath);
    buttonLoadAtlas->SetButton({ 60.f,30.f }, { windowSize.x * 0.1f, windowSize.y * 0.2f }, sf::Color::White, Origins::MC);
    buttonLoadAtlas->SetButtonText(font, "Load\nSprite Atlas", 12.f, sf::Color::Black, { windowSize.x * 0.1f, windowSize.y * 0.2f }, Origins::MC);
    AddGameObject(buttonLoadAtlas, Layers::Ui);

    spriteSheet = new SpriteGo("spritesheet");
    spriteSheet->Init();

    textFPS = new TextGo("textfps");
    textFPS->Set(font, "FPS", 20, sf::Color::White);
    textFPS->SetPosition(windowSize.x * 0.1f, windowSize.y * 0.25f);
    textFPS->SetOrigin(Origins::MR);
    AddGameObject(textFPS, Layers::Ui);

    inputfieldFPS = new InputField("inputfieldfps");
    inputfieldFPS->SetPosition(windowSize.x * 0.11f, windowSize.y * 0.27f);
    inputfieldFPS->SetOrigin(Origins::ML);
    AddGameObject(inputfieldFPS, Layers::Ui);

    buttonStop = new Button(Button::ButtonIdentifier::stop, "buttonstop");
    buttonStop->SetButton({ 30.f,30.f }, { windowSize.x * 0.1f, windowSize.y * 0.5f }, sf::Color::White, Origins::MC);
    buttonStop->SetButtonText(font, "Stop", 12.f, sf::Color::Black, { windowSize.x * 0.1f, windowSize.y * 0.5f }, Origins::MC);
    AddGameObject(buttonStop, Layers::Ui);

    buttonPlay = new Button(Button::ButtonIdentifier::play, "buttonplay");
    buttonPlay->SetButton({ 30.f,30.f }, { windowSize.x * 0.15f, windowSize.y * 0.5f }, sf::Color::White, Origins::MC);
    buttonPlay->SetButtonText(font, "Play", 12.f, sf::Color::Black, { windowSize.x * 0.15f, windowSize.y * 0.5f }, Origins::MC);
    AddGameObject(buttonPlay, Layers::Ui);

    buttonSaveAnimation = new Button(Button::ButtonIdentifier::save, "buttonsave");
    buttonSaveAnimation->SetButton({ 80.f,40.f }, { windowSize.x * 0.8f, windowSize.y * 0.85f }, sf::Color::White, Origins::MC);
    buttonSaveAnimation->SetButtonText(font, "Save\nAnimation", 15.f, sf::Color::Black, { windowSize.x * 0.8f, windowSize.y * 0.85f }, Origins::MC);
    AddGameObject(buttonSaveAnimation, Layers::Ui);

    editorBorder.setOutlineColor(sf::Color::Red);
    editorBorder.setFillColor(sf::Color::Transparent);
    editorBorder.setOutlineThickness(2.f);
    editorBorder.setPosition(windowSize.x * 0.2f, windowSize.y * 0.2f);
    editorBorder.setSize({ windowSize.x * 0.6f, windowSize.y * 0.6f });

    textureBorder.setOutlineColor(sf::Color::Cyan);
    textureBorder.setFillColor(sf::Color::Transparent);
    textureBorder.setOutlineThickness(5.f);



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

void SceneAnimationTool::UpdateEvent(const sf::Event& event)
{
    Scene::UpdateEvent(event);
    switch (event.type)
    {
    case  sf::Event::MouseWheelScrolled:
    {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
        {
            float zoomAmount = 0.1f;
            if (event.mouseWheelScroll.delta > 0)
            {
                worldView.zoom(1.f - zoomAmount);
            }
            else
            {
                worldView.zoom(1.f + zoomAmount);
            }
        }
    }
    break;
    case sf::Event::MouseButtonPressed:
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mouseWorldPos = ScreenToWorld((sf::Vector2i)InputManager::GetMousePos());
            if (IsWithinWorldView(mouseWorldPos))
            {
                isLeftDragging = true;
                leftDragStartPos = mouseWorldPos;
            }
        }
        else if (event.mouseButton.button == sf::Mouse::Right)
        {
            isRightDragging = true;
            rightDragStartPos = ScreenToWorld((sf::Vector2i)InputManager::GetMousePos());
            lastMousePos = rightDragStartPos;
        }
    }
    break;
    case sf::Event::MouseButtonReleased:
    {
        if (event.mouseButton.button == sf::Mouse::Left && isLeftDragging && isAtlasPath)
        {
            isLeftDragging = false;
            sf::Vector2f endPos = ScreenToWorld((sf::Vector2i)InputManager::GetMousePos());
            if (IsWithinWorldView(endPos))
            {
                sf::FloatRect selectedArea(leftDragStartPos, endPos - leftDragStartPos);
                selectedAreas.push_back(selectedArea);

                std::cout << selectedArea.left
                    << " : " << selectedArea.top
                    << " : " << selectedArea.width
                    << " : " << selectedArea.height << std::endl;
            }
        }
        else if (event.mouseButton.button == sf::Mouse::Right)
        {
            isRightDragging = false;
        }
    }
    break;

    case sf::Event::MouseMoved:
    {
        if (isRightDragging)
        {
            sf::Vector2f currentMousePos = ScreenToWorld((sf::Vector2i)InputManager::GetMousePos());
            sf::Vector2f delta = lastMousePos - currentMousePos;
            worldView.move(delta);
            lastMousePos = currentMousePos;
        }
        break;
    }
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
        spriteSheet->SetOrigin(Origins::TL);
        selectedAreas.clear();

        textureBorder.setPosition(0, 0);
        textureBorder.setSize((sf::Vector2f)spriteSheet->GetTexture()->getSize());

        if (firstTextureLoad)
        {
            firstTextureLoad = false;
			AddGameObject(spriteSheet);
        }
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

    const sf::View& saveView = window.getView();
    window.setView(uiView);
    window.draw(editorBorder);
    
    window.setView(worldView);
    window.draw(textureBorder);
    window.setView(saveView);

    for (const auto& area : selectedAreas)
    {
        sf::RectangleShape rectangle;
        rectangle.setPosition(sf::Vector2f(area.left, area.top));
        rectangle.setSize(sf::Vector2f(area.width, area.height));
        rectangle.setFillColor(sf::Color::Transparent);
        rectangle.setOutlineColor(sf::Color::Green);
        rectangle.setOutlineThickness(2.0f);

        window.setView(worldView);
        window.draw(rectangle);
    }
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

bool SceneAnimationTool::IsWithinWorldView(const sf::Vector2f& point)
{
    sf::FloatRect viewBounds(worldView.getCenter() - worldView.getSize() / 2.f, worldView.getSize());
    return viewBounds.contains(point);
}
