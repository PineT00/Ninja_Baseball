#include "pch.h"
#include "Spawner.h"

Spawner::Spawner(const std::string& name)
	: GameObject(name)
{
}

GameObject* Spawner::Create()
{
	return nullptr;
}

void Spawner::Init()
{
	GameObject::Init();
}

void Spawner::Release()
{
	GameObject::Release();
}

void Spawner::Reset()
{
	GameObject::Reset();

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetCurrentScene());
}

void Spawner::Update(float dt)
{
	GameObject::Update(dt);

	timer += dt;

	if (timer > interval)
	{
		timer = 0.f;

		for (int i = 0; i < spawnCount; ++i)
		{
			sf::Vector2f spawnPosition = { Utils::Random::RandomRange(-220.f, 220.f), -550.f };

			GameObject* newGo = Create();
			if (newGo == nullptr) return;
			newGo->Init();
			newGo->Reset();
			newGo->SetPosition(spawnPosition);
			//sceneGame->enemyList.push_back(dynamic_cast<Enemy*>(newGo));
			sceneGame->AddGameObject(newGo);
		}
	}
}