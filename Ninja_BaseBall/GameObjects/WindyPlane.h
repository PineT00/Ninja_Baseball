#pragma once
#include "SpriteGo.h"
#include "Animator.h"
#include "BossTable.h"

class Player;
class SceneDevBoss;

class WindyPlane : public SpriteGo
{
public :
	
	struct ClipInfo
	{
		std::string chase;
		std::string punchOne;
		std::string punchTwo;
		std::string windAttack;
		std::string gunAttack;
		std::string crying;
		std::string dead;
		bool isChase = false;
		bool isPunchOne = false;
		bool isPunchTwo = false;
		bool isWindAttack = false;
		bool isGunAttack = false;
		bool isCrying = false;
		bool isDead = false;

		ClipInfo()
		{
		}
		ClipInfo(
			const std::string& chase, 
			const std::string& punchOne, 
			const std::string& punchTwo, 
			const std::string& windAttack, 
			const std::string& gunAttack, 
			const std::string& crying, 
			const std::string& dead, 
			bool isChase, bool isPunchOne, bool isPunchTwo, bool isWindAttack,
			bool isGunAttack, bool isCrying, bool isDead)
			: chase(chase), punchOne(punchOne), punchTwo(punchTwo), windAttack(windAttack), gunAttack(gunAttack), crying(crying), dead(dead),
			isChase(isChase), isPunchOne(isPunchOne), isPunchTwo(isPunchTwo), isWindAttack(isWindAttack), isGunAttack(isGunAttack), isCrying(isCrying), isDead(isDead)
		{
		}
	};


	enum class Status
	{
		IDLE,
		CHASE,
		PUNCHONESHOT,
		PUNCHTWOSHOT,
		WINDATTACK,
		GUNATTACK,
		CRYING,
		DEAD,
	};

	const int statusCount = 9;

protected:
	WindyPlane(const WindyPlane&) = delete;
	WindyPlane(WindyPlane&&) = delete;
	WindyPlane& operator=(const WindyPlane&) = delete;
	WindyPlane& operator=(WindyPlane&&) = delete;

	BossData data;
	SceneDevBoss* sceneDevBoss = nullptr;
	Player* player = nullptr;
	Animator animator;
	ClipInfo currentClipInfo;
	Status currentStatus = Status::IDLE;

	std::string currentClipId;
	std::vector<ClipInfo> clipInfos;
	std::vector<std::function<void()>> bossAttackPatterns;
	std::vector<std::function<void()>> bossMovePatterns;
	std::vector<std::string> clipIds;
	
	sf::Vector2f windowSize;
	sf::Vector2f direction = { 1.f, 0.f };

	float speed = 200.f;
	float findTimer = 0.f;
	float findInterval = 3.f;
	bool isAlive = true;

public:
	WindyPlane(const std::string& name = "windyplane");
	~WindyPlane() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window);

	// ���� ������ ���ϵ�
	void ChasePlayer(float dt);

	// ���� ���� ���ϵ�
	void PunchOneTime();

	// �÷��̾� ã��
	void FindPlayer();

	// �ǰ�
	void OnDamaged(float damage);
	void OnDie();

	// ����

};