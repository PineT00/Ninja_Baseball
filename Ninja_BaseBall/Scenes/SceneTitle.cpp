#include "pch.h"
#include "SceneTitle.h"
#include "TextGo.h"
#include "SpriteGo.h"

SceneTitle::SceneTitle(SceneIDs id)
	: Scene(id)
{
	windowSize = (sf::Vector2f)FRAMEWORK.GetWindowSize();
}

void SceneTitle::Init()
{
	font = *FONT_MANAGER.GetResource("fonts/retroGaming.ttf");

	worldView.setSize(windowSize);
	worldView.setCenter(0, 0);
	uiView.setSize(windowSize);
	uiView.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);

	backgroundBrick = new SpriteGo("backgroundbrick");
	backgroundBrick->SetTexture("graphics/6_UI/title/title_background.bmp");
	backgroundBrick->SetOrigin(Origins::MC);
	AddGameObject(backgroundBrick);

	backgroundShadow = new SpriteGo("backgroundshadow");
	backgroundShadow->SetTexture("graphics/6_UI/title/silhouette.bmp");
	backgroundShadow->SetOrigin({ backgroundShadow->GetTexture()->getSize().x * 0.5f,backgroundShadow->GetTexture()->getSize().y * 0.7f });
	AddGameObject(backgroundShadow);

	titleName = new SpriteGo("titlename");
	titleName->SetTexture("graphics/6_UI/title/titleName.bmp");
	titleName->SetOrigin({titleName->GetTexture()->getSize().x * 0.5f,titleName->GetTexture()->getSize().y * 0.8f });
	AddGameObject(titleName);

	insertCoin = new SpriteGo("insertcoin");
	insertCoin->SetTexture("graphics/6_UI/title/push_any_button.bmp");
	insertCoin->SetPosition({ 0, 100 });
	insertCoin->SetOrigin(Origins::MC);
	AddGameObject(insertCoin);

	textDescriptions.push_back(new TextGo("textcoin"));
	textDescriptions.push_back(new TextGo("textcorpname"));
	textDescriptions.push_back(new TextGo("textcredit"));

	for (int i = 0; i < descriptions.size(); ++i)
	{
		textDescriptions[i]->Set(font, descriptions[i], 30, sf::Color::White);
		textDescriptions[i]->SetPosition({ 0, 200.f + (60.f * i) });
		textDescriptions[i]->SetOrigin(Origins::MC);
		AddGameObject(textDescriptions[i]);
	}

	Scene::Init();
}

void SceneTitle::Release()
{
	Scene::Release();
}

void SceneTitle::Enter()
{
	Scene::Enter();
}

void SceneTitle::Exit()
{
	FRAMEWORK.SetTimeScale(1.f);

	Scene::Exit();
}

void SceneTitle::Update(float dt)
{
	Scene::Update(dt);

	time += dt;

	BlinkIntro(0.3f);

	if (InputManager::GetKeyDown(sf::Keyboard::Enter))
	{
		SCENE_MANAGER.ChangeScene(SceneIDs::SceneDev1);
	}
}

void SceneTitle::BlinkIntro(float blinkTime)
{
	this->blinkTime = blinkTime;

	if (time >= 0 && time < blinkTime)
	{
		insertCoin->SetActive(true);
	}
	else if (time >= blinkTime && time < blinkTime * 2)
	{
		insertCoin->SetActive(false);
	}
	else if (time >= blinkTime * 2)
	{
		time = 0.f;
	}
}
