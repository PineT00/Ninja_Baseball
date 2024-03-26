#pragma once
#include "Scene.h"

class Tilemap;
class UiHUD;
class SpriteGo;
class TextGo;
class InputField;
class YellowBaseBall;
class Stage1;
class Player;
class Player2;
class Enemy;
class WindyPlane;

class SceneDev1 : public Scene
{
public:
	Player* player = nullptr;
	Enemy* enemy = nullptr;

	bool cameraShakeOn = false;
	float cameraShakeTime = 1.f;

	bool isFighting = false;

	sf::Vector2f worldViewCenter = {0.f,0.f};

	sf::FloatRect stageRect;

	int currStage = 0;

	Stage1* stage = nullptr;

	bool enterToBossFloor = false;

	//���� �������� ī�޶� �߽�
	float camCenter1;
	float camCenter2;
	float camCenter3;
	float camCenter4; //�븻
	float camCenter5; //�μ���1
	float camCenter6; //�μ���2
	float camCenter7; //����� ��������
	float camCenter8; //����� ����

protected :
	SceneDev1(const SceneDev1&)				= delete;
	SceneDev1(SceneDev1&&)					= delete;
	SceneDev1& operator=(const SceneDev1&)	= delete;
	SceneDev1& operator=(SceneDev1&&)		= delete;	
	
	sf::RectangleShape cameraRect;

	float xMax = 0.f;

	WindyPlane* windyPlane = nullptr;

	
	UiHUD* hud = nullptr;
	// YellowBaseBall* yellowEnemy;
	// YellowBaseBall* yellowEnemy2;
	sf::Vector2f windowSize;

private:
	struct MonsterInfo
	{
		sf::FloatRect damageBox;
		std::shared_ptr<Enemy> monster;
	};
	std::vector<MonsterInfo> monsterInfos;
	std::vector<Enemy*> enemies;
	

public:
	std::vector<Enemy*> GetEnemyList() { return enemies; }

	SceneDev1(SceneIDs id);
	~SceneDev1() override = default;
	sf::Vector2f ClampByTileMap(const sf::Vector2f point);

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
	void SpawnEnemy(const std::string& type,const sf::Vector2f& position);
	void CameraShake(float dt);

	void FightOn();
	void ClearStage();
	void MoveToBoss();
};