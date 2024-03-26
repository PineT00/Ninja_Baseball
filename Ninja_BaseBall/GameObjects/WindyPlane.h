#pragma once
#include "SpriteGo.h"
#include "Animator.h"
#include "BossTable.h"
#include "Enemy.h"

class WindEffect;
class Player;
class SceneDev1;
struct BossData;

class WindyPlane : public Enemy
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
		DEATH,
		HOLD,
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
	const int EFFECTS_COUNT = 30;

protected:
	WindyPlane(const WindyPlane&) = delete;
	WindyPlane(WindyPlane&&) = delete;
	WindyPlane& operator=(const WindyPlane&) = delete;
	WindyPlane& operator=(WindyPlane&&) = delete;

	SceneDev1* sceneDev1 = nullptr;
	Player* player = nullptr;
	Animator animator;

	ClipInfo currentClipInfo;
	BossPartsStatus currentPartsStatus = BossPartsStatus::Wing;
	WindyPlaneStatus currentStatus = WindyPlaneStatus::IDLE;
	EnemyType enemyType = EnemyType::Boss;

	std::string currentClipId;

	std::unordered_map<BossPartsStatus, BossData>& data;
	std::vector<ClipInfo> clipInfos;
	std::vector<WindEffect*> windEffects;
	std::vector<std::string> clipIds;
	
	sf::Vector2f windowSize;
	sf::Vector2f direction = { 1.f, 0.f };
	sf::Vector2f windDirection;
	sf::Vector2f up = { 0, -1.f };
	
	sf::RectangleShape hitBox;
	sf::RectangleShape closeAttackBox;
	sf::RectangleShape uppercutAttackBox;
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
	bool isAwake = true;


public:
	WindyPlane(const std::string& name = "windyplane");
	~WindyPlane() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window);

	void SetFlipX(bool flipX) override;

	// 보스 움직임 패턴들
	void ChasePlayer(float dt);

	// 보스 공격 패턴들
	void AttackOneTwo();
	void AttackStraight();
	void AttackUpperCut();
	void AttackWind(float dt);
	void AttackGun(); // NoWing은 AttackGunReady -> AttackGun
	void AttackGunReady();
	
	// 보스 공격 패턴 이벤트들
	void AttackOneTwoEvent();
	void AttackStraightEvent();
	void AttackUpperCutEvent();
	void AttackWindEvent();
	void AttackGunEvent();
	void AttackGunReadyEvent();

	void ApplyAttackEvent(bool isClosed, bool isRanged);

	// 플레이어 찾기
	void FindPlayer();

	// 피격
	void OnDamage(int damage, int count = -1) override; 
	void OnDamagedEvent();
	void OnDie();
	void OnDieEvent();
	void HoldEvent();

	// 상태
	void LoadAllEvents();
	void CheckEndFrame();
	void PlayAnimation(BossPartsStatus status, WindyPlaneStatus planeStatus);
	void SetCurrentStatus(WindyPlaneStatus status) { currentStatus = status; }
	void HoldAction();

	sf::FloatRect GetDamageBox()  const override { return hitBox.getGlobalBounds(); }
};