#include "pch.h"
#include "UiHUD.h"
#include "SceneGame.h"
#include "Player.h"

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

	player1_Frame = new SpriteGo;
	player1_Name = new SpriteGo;

	sf::Font& font = *FONT_MANAGER.GetResource("fonts/retroGaming.ttf");
	float textSize = 60.f;

	textFps.Set(font, "", textSize, sf::Color::White);
	textFps.SetActive(false);
	textFps.SetPosition({ referenceResolution.x, 0.f });

	player1_Frame->SetTexture("graphics/6_UI/inGame/player.bmp");
	player1_Name->SetTexture("graphics/6_UI/inGame/red_nameJOSE.bmp");
	//player1_Portrait;

	player1_hpRedBack.SetTexture("graphics/6_UI/inGame/hp_red.png");
	player1_hpBar.SetTexture("graphics/6_UI/inGame/hp_yellow.png");
	

	player1_Frame->Init();
	player1_Name->Init();

	uiAnimator.SetTarget(&player1_Portrait);
	gameOverAnimator.SetTarget(&gameOverScreen);

	//sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetScene(SceneIDs::SceneGame));
}

void UiHUD::Reset()
{
	sf::Vector2f winSizeF = (sf::Vector2f)windowSize;

	font = *FONT_MANAGER.GetResource("fonts/retroGaming.ttf");

	GameObject::Reset();

	player1_Frame->SetPosition({ 120.f, 15.f });
	player1_Name->SetPosition({ 190.f, 15.f });
	player1_Score.SetPosition({ 300.f, 30.f });
	player1_Life.SetPosition({ 300.f, 53.f });
	player1_Portrait.setPosition({ 155.f, 75.f });
	player1_hpRedBack.SetPosition({ 190.f, 87.f });
	player1_hpBar.SetPosition({ 190.f, 87.f });

	uiAnimator.Play("animations/ui/player1_Portrait.csv");


	player = dynamic_cast<Player*>(SCENE_MANAGER.GetCurrentScene()->FindGameObject("Player"));

	score = player->score;
	life = player->life;

	player1_Score.Set(font, std::to_string(score), 30, sf::Color::Yellow);
	player1_Life.Set(font, std::to_string(life), 30, sf::Color::Yellow);

	player1_prevHP = player->maxHp;

	gameOverScreen.setPosition(winSizeF/2.f);
	gameOverCount = 10;
	gameOver = false;
	gameOverTimer.Set(font, std::to_string(gameOverCount), countTextSize, sf::Color::Red);
	gameOverTimer.SetOrigin(Origins::MC);
	gameOverTimer.SetPosition(winSizeF / 2.f);
	gameOverTimer.SetActive(false);
}

void UiHUD::Update(float dt)
{
	GameObject::Update(dt);
	uiAnimator.Update(dt);

	if (gameOver)
	{
		gameOverAnimator.Update(dt);

		if (countTextSize < 300.f)
		{
			countTextSize += 5.f * dt;
		}

		gameOverTimer.SetTextSize(countTextSize);

		timer -= dt;
		if (timer <= 0.f)
		{
			timer = 0.5f;
			gameOverCount -= 1;
		}

		gameOverTimer.SetText(std::to_string(std::max(0, gameOverCount)));
	}


	float currentHp = player->hp;
	if (currentHp < 0) currentHp = 0;
	float maxHp = player->maxHp;
	sf::Vector2u rectSize = player1_hpBar.GetTexture()->getSize();

	if (player->currStatus == Player::Status::isHitted)
	{
		uiAnimator.Play("animations/ui/player1_PortraitHurt.csv");
	}
	else
	{
		uiAnimator.PlayQueue("animations/ui/player1_Portrait.csv");
	}
	player1_hpBar.SetTextureRect({ 0,0,(int)((currentHp / maxHp) * rectSize.x), (int)rectSize.y });

	score = player->score;
	life = player->life;
	player1_Score.Set(font, std::to_string(score), 30, sf::Color::Yellow);
	player1_Life.Set(font, std::to_string(life), 30, sf::Color::Yellow);
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
	player1_Frame->Draw(window);
	player1_Name->Draw(window);
	player1_Score.Draw(window);
	player1_Life.Draw(window);
	player1_hpRedBack.Draw(window);
	player1_hpBar.Draw(window);
	window.draw(player1_Portrait);

	GameObject::Draw(window);
	if (SCENE_MANAGER.GetDeveloperMode())
	{
		SetFps(FRAMEWORK.GetFps());
		textFps.Draw(window);
	}

	if (gameOver)
	{
		window.draw(gameOverScreen);
		gameOverTimer.Draw(window);
	}
}

void UiHUD::GameOverCount()
{
	// ¿ø·¡ 3ÃÊ
	timer = 0.5f;
	gameOver = true;
	gameOverTimer.SetActive(true);
	gameOverAnimator.Play("animations/ui/gameOverScreen.csv");
}

void UiHUD::ResetGameOver()
{
	gameOver = false;
	gameOverCount = 10;
}
