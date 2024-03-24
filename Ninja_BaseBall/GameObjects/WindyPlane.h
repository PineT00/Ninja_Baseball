#pragma once
#include "SpriteGo.h"
#include "Animator.h"
#include "BossTable.h"

class Player;
class SceneDevBoss;
struct BossData;

class WindyPlane : public SpriteGo
{
public :
	enum class WindyPlaneStatus
	{
		IDLE,
		DAMAGED,
		ONETWO,
		STRAIGHT,
		UPPERCUT,
		WIND,
		GUN,
		GUNREADY,
		FINAL,
		DEATH
	};

	struct ClipInfo
	{
		BossPartsStatus partsStatus;
		std::vector <std::string> clips;
		std::vector<bool> clipStatus;

		ClipInfo()
		{
		}

		ClipInfo(BossPartsStatus partsStatus, const std::vector<std::string>& clips, const std::vector<bool>& clipStatus)
			: partsStatus(partsStatus), clips(std::move(clips)), clipStatus(std::move(clipStatus))
		{
		}
	};

	const int PARTS_STATUS_COUNT = 5;

protected:
	WindyPlane(const WindyPlane&) = delete;
	WindyPlane(WindyPlane&&) = delete;
	WindyPlane& operator=(const WindyPlane&) = delete;
	WindyPlane& operator=(WindyPlane&&) = delete;

	SceneDevBoss* sceneDevBoss = nullptr;
	Player* player = nullptr;
	Animator animator;
	Animator effectAnimator;
	ClipInfo currentClipInfo;
	BossPartsStatus currentPartsStatus = BossPartsStatus::Wing;
	WindyPlaneStatus currentStatus = WindyPlaneStatus::IDLE;

	std::string currentClipId;

	std::unordered_map<BossPartsStatus, BossData>& data;
	std::vector<ClipInfo> clipInfos;
	std::vector<std::function<void()>> bossAttackPatterns;
	std::vector<std::function<void()>> bossMovePatterns;
	std::vector<std::string> clipIds;
	
	sf::Vector2f windowSize;
	sf::Vector2f direction = { 1.f, 0.f };
	sf::Vector2f windDirection;
	sf::Vector2f up = { 0, -1.f };
	
	sf::Sprite spriteEffect;

	sf::RectangleShape hitBox;
	sf::RectangleShape attackBox;
	sf::RectangleShape rangedAttackBox;

	float speed = 100.f;
	float currentSpeed = speed;
	float findTimer = 0.f;
	float findInterval = 3.f;
	float statusTimer = 0.f;
	float statusInterval = 1.f;

	float maxHp = 2000.f;
	float hp = maxHp;

	int hitCount = 0;

	bool isAlive = true;
	bool isAscending = false; // 현재 상승 중인지 여부



public:
	WindyPlane(const std::string& name = "windyplane");
	~WindyPlane() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window);

	void SetFlipX(bool flipX) override;

	// Test
	void UpdateWindAttack(float dt);

	// 보스 움직임 패턴들
	void ChasePlayer(float dt);

	// 보스 공격 패턴들
	void AttackOneTwo();
	void AttackStraight();
	void AttackUpperCut();
	void AttackWind();
	void AttackGun(); // NoWing은 AttackGunReady -> AttackGun
	void AttackGunReady();
	
	// 보스 공격 패턴 이벤트들
	void AttackOneTwoEvent();
	void AttackStraightEvent();
	void AttackUpperCutEvent();
	void AttackWindEvent();
	void AttackGunEvent();
	void AttackGunReadyEvent();

	void ApplyAttackEvent(bool isRanged);


	// 플레이어 찾기
	void FindPlayer();

	// 피격
	void OnDamaged(float damage);
	void OnDamagedEvent();
	void OnDie();
	void OnDieEvent();

	// 상태
	//void LoadAllEvents();
	void PlayAnimation(BossPartsStatus status, WindyPlaneStatus planeStatus);
	void CheckEndFrame();
};