#include "pch.h"
#include "WindyPlane.h"
#include "Player.h"
#include "SceneDevBoss.h"
#include "rapidcsv.h"

WindyPlane::WindyPlane(const std::string& name)
	: SpriteGo(name)
{
	data = BOSS_TABLE->Get(BossType::WINDYPLANE);
}

void WindyPlane::Init()
{
	SpriteGo::Init();
	animator.SetTarget(&sprite);

	// 데이터 테이블화 필요
	//const std::string& chase
	//const std::string& punchOne
	//const std::string& punchTwo
	//const std::string& windAttack
	//const std::string& gunAttack
	//const std::string& crying
	//const std::string& dead, 
	//bool isChase, bool isPunchOne, bool isPunchTwo, bool isWindAttack,
	//bool isGunAttack, bool isCrying, bool isDead

	clipInfos.push_back({ data.animationChaseClipId, data.animationPunchOneClipId, data.animationPunchTwoClipId, data.animationWindAttackClipId, data.animationGunAttackClipId,	data.animationCryingClipId,	data.animationDeadClipId,
		true,false,false,false,false,false,false });
	clipInfos.push_back({ data.animationChaseClipId, data.animationPunchOneClipId, data.animationPunchTwoClipId, data.animationWindAttackClipId, data.animationGunAttackClipId,	data.animationCryingClipId,	data.animationDeadClipId,
		false,true,false,false,false,false,false });
	clipInfos.push_back({ data.animationChaseClipId, data.animationPunchOneClipId, data.animationPunchTwoClipId, data.animationWindAttackClipId, data.animationGunAttackClipId,	data.animationCryingClipId,	data.animationDeadClipId,
		false,false,true,false,false,false,false });
	clipInfos.push_back({ data.animationChaseClipId, data.animationPunchOneClipId, data.animationPunchTwoClipId, data.animationWindAttackClipId, data.animationGunAttackClipId,	data.animationCryingClipId,	data.animationDeadClipId,
		false,false,false,true,false,false,false });
	clipInfos.push_back({ data.animationChaseClipId, data.animationPunchOneClipId, data.animationPunchTwoClipId, data.animationWindAttackClipId, data.animationGunAttackClipId,	data.animationCryingClipId,	data.animationDeadClipId,
		false,false,false,false,true,false,false });
	clipInfos.push_back({ data.animationChaseClipId, data.animationPunchOneClipId, data.animationPunchTwoClipId, data.animationWindAttackClipId, data.animationGunAttackClipId,	data.animationCryingClipId,	data.animationDeadClipId,
		false,false,false,false,false,true,false });
	clipInfos.push_back({ data.animationChaseClipId, data.animationPunchOneClipId, data.animationPunchTwoClipId, data.animationWindAttackClipId, data.animationGunAttackClipId,	data.animationCryingClipId,	data.animationDeadClipId,
		false,false,false,false,false,false,true});
	
	// Phase1
	clipIds.push_back("animations/Boss/Phase1/bossIdle.csv");
	clipIds.push_back("animations/Boss/Phase1/bossDashPunchOneShot.csv");
	clipIds.push_back("animations/Boss/Phase1/bossShootGun.csv");
	clipIds.push_back("animations/Boss/Phase1/bossOnDamaged.csv");
	clipIds.push_back("animations/Boss/Phase1/bossPunchOneShot.csv");
	clipIds.push_back("animations/Boss/Phase1/bossPunchTwoShot");
	clipIds.push_back("animations/Boss/Phase1/bossWindShot.csv");


	// Phase2



	// FindGo
	sceneDevBoss = dynamic_cast<SceneDevBoss*>(SCENE_MANAGER.GetScene(SceneIDs::SceneDevBoss));
	player = dynamic_cast<Player*>(sceneDevBoss->FindGameObject("player"));
	

}

void WindyPlane::Reset()
{
	animator.ClearEvent();
	animator.Play(clipIds[0]);
	SetOrigin(Origins::BC);
}

void WindyPlane::Update(float dt)
{
	SpriteGo::Update(dt);
	animator.Update(dt);

	ChasePlayer(dt);

	// Test Code
	if (InputManager::GetKeyUp(sf::Keyboard::Num1))
	{
		currentStatus = Status::IDLE;
	}
	else if (InputManager::GetKeyUp(sf::Keyboard::Num2))
	{
		currentStatus = Status::GUNATTACK;
	}
	else if (InputManager::GetKeyUp(sf::Keyboard::Num3))
	{
		currentStatus = Status::WINDATTACK;
	}

	currentClipInfo = clipInfos[(int)currentStatus];

	if (currentClipInfo.isChase) currentClipId = currentClipInfo.chase;
	else if (currentClipInfo.isPunchOne) currentClipId = currentClipInfo.punchOne;
	else if (currentClipInfo.isPunchTwo) currentClipId = currentClipInfo.punchTwo;
	else if (currentClipInfo.isWindAttack) currentClipId = currentClipInfo.windAttack;
	else if (currentClipInfo.isGunAttack) currentClipId = currentClipInfo.gunAttack;
	else if (currentClipInfo.isCrying) currentClipId = currentClipInfo.crying;
	else if (currentClipInfo.isDead) currentClipId = currentClipInfo.dead;

	if (animator.GetCurrentClipId() != currentClipId)
	{
		animator.Play(currentClipId);
	}


}

void WindyPlane::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void WindyPlane::ChasePlayer(float dt)
{
	findTimer += dt;

	if (findTimer >= findInterval)
	{
		findTimer = 0.f;
		FindPlayer();
	}

	if (abs(player->GetPosition().y - position.y) < 20)
	{
		direction = Utils::MyMath::GetNormal(player->GetPosition() - position);
	}

	direction.x < 0 ? SetFlipX(true) : SetFlipX(false);

	Translate(direction * speed * dt);
}

void WindyPlane::PunchOneTime()
{
}

void WindyPlane::FindPlayer()
{
	direction = Utils::MyMath::GetNormal(player->GetPosition() - position);
}
