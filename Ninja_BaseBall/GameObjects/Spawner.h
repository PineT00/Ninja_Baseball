#pragma once
#include "GameObject.h"
#include "SceneGame.h"

class SceneGame;
class Background;

class Spawner : public GameObject
{
protected :
	Spawner(const Spawner&)			   = delete;
	Spawner(Spawner&&)				   = delete;
	Spawner& operator=(const Spawner&) = delete;
	Spawner& operator=(Spawner&&)	   = delete;

	SceneGame* sceneGame = nullptr;
	Background* background = nullptr;

	float timer = 0.f;

	float interval = 1.f;
	float intervalMin;
	float intervalMax;

	int spawnCount = 3.f;
	int spawnCountMin;
	int spawnCountMax;

	float radius = 250.f;
	virtual GameObject* Create() = 0;

public :
	Spawner(const std::string& name = "");
	~Spawner() override				 = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
};