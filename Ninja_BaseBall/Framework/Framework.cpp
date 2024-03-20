#include "pch.h"
#include "Framework.h"

void Framework::Init(int width, int height, const std::string& name)
{
    srand(std::time(NULL));

    windowSize.x = width;
    windowSize.y = height;


    window.create(sf::VideoMode(windowSize.x, windowSize.y), name);

    DATATABLE_MANAGER.Init();
    SCENE_MANAGER.Init();
    InputManager::Init();
}

void Framework::Do()
{
    sf::Clock fpsClock;
    int frameCount = 0;

    while (window.isOpen())
    {
        deltaTime = realDeltaTime = clock.restart();
        deltaTime *= timeScale;

        time += deltaTime;
        realTime += realDeltaTime;

        fixedDeltaTime += deltaTime;


        fpsTimer += realDeltaTime.asSeconds();
        fpsCount++;
        if (fpsTimer >= 1.f)
        {
            fpsTimer = 0.f;
            fps = fpsCount;
            fpsCount = 0;
        }


        InputManager::Clear();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            SCENE_MANAGER.UpdateEvent(event);
            InputManager::UpdateEvent(event); // 키 입력 이벤트 처리
        }

        //Update
        InputManager::Update(GetDeltaTime());
        if (SCENE_MANAGER.Update(GetDeltaTime()))
        {
            SCENE_MANAGER.LateUpdate(GetDeltaTime());

            float fdt = fixedDeltaTime.asSeconds();

            if (fdt > fixedUpdateTime)
            {
                SCENE_MANAGER.FixeUpdate(fdt);
                fixedDeltaTime = sf::Time::Zero;
            }
        }

        // Draw
        window.clear();
        SCENE_MANAGER.Draw(window);
        window.display();
    }
}

void Framework::Release()
{
    SCENE_MANAGER.Release();
    TEXTURE_MANAGER.UnloadAll();
    FONT_MANAGER.UnloadAll();
    SOUND_BUFFER_MANAGER.UnloadAll();
}