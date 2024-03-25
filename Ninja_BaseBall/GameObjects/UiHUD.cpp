#include "pch.h"
#include "UiHUD.h"
#include "SceneGame.h"

UiHUD::UiHUD(const std::string& name)
	: GameObject(name)
{
	windowSize = FRAMEWORK.GetWindowSize();
}

void UiHUD::SetFps(int fps)
{
	textFps.SetText(formatFps + std::to_string(fps));
}

void UiHUD::Init()
{
	GameObject::Init();
	textFps.Init();

	sf::Font& font = *FONT_MANAGER.GetResource("fonts/retroGaming.ttf");
	float textSize = 60.f;

	textFps.Set(font, "", textSize, sf::Color::White);
	textFps.SetActive(false);
	textFps.SetPosition({ referenceResolution.x, 0.f });

	player1_Frame.SetTexture("graphics/6_UI/inGame/player.bmp");
	player1_Name.SetTexture("graphics/6_UI/inGame/red_nameJOSE.bmp");
	//player1_Portrait;

	player1_Score.Set(font,"0",10, sf::Color::Yellow);
	player1_Life;



	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetScene(SceneIDs::SceneGame));
}

void UiHUD::Reset()
{
	GameObject::Reset();

}

void UiHUD::Update(float dt)
{
	GameObject::Update(dt);
}

void UiHUD::LateUpdate(float dt)
{
	GameObject::LateUpdate(dt);
}

void UiHUD::FixedUpdate(float dt)
{
	GameObject::FixedUpdate(dt);
}

void UiHUD::Draw(sf::RenderWindow& window)
{
	GameObject::Draw(window);
	if (SCENE_MANAGER.GetDeveloperMode())
	{
		SetFps(FRAMEWORK.GetFps());
		textFps.Draw(window);
	}
}