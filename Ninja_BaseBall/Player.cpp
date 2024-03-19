#include "pch.h"
#include "Player.h"
#include "SceneDev1.h"

Player::Player(const std::string& name)
{
}

Player::~Player()
{
}

void Player::TestInstance()
{
	std::cout << "TestInstance()" << std::endl;
}

void Player::TestStatic()
{
	std::cout << "TestStatic()" << std::endl;
}

void Player::Init()
{
	SpriteGo::Init();

	//animator.AddClip(RES_MGR_ANI_CLIP.Get("Animations/idle.csv"));
	//animator.AddClip(RES_MGR_ANI_CLIP.Get("Animations/Jump.csv"));
	//animator.AddClip(RES_MGR_ANI_CLIP.Get("Animations/Run.csv"));

	animator.SetTarget(&sprite);

	hasHitBox = true;
}

void Player::Reset()
{
	animator.ClearEvent();

	std::function<void()> funcInstance = std::bind(&Player::TestInstance, this);
	animator.AddEvent("animations/Jump.csv", 5, funcInstance);

	std::function<void()> funcStatic = std::bind(&Player::TestStatic);
	animator.AddEvent("animations/Idle.csv", 5, funcStatic);

	animator.Play("animations/Idle.csv");
	SetOrigin(Origins::BC);
	//SetPosition({ 0.f, 0.f });
	SetScale({ 0.8f,0.8f });

	sceneDev1 = dynamic_cast<SceneDev1*>(SCENE_MANAGER.GetCurrentScene());
}

void Player::Update(float dt)
{
	//SpriteGo::Update(dt);
	animator.Update(dt);

	float h = InputManager::GetAxis(Axis::Horizontal);
	float v = InputManager::GetAxis(Axis::Vertical);

	if (isGrounded && InputManager::GetKeyDown(sf::Keyboard::Space))
	{
		isGrounded = false;
		animator.Play("animations/Jump.csv");
	}

	velocity.x = h * speed;
	velocity.y = v * speed;

	position += velocity * dt;

	if (sceneDev1 != nullptr)
	{
		position = sceneDev1->ClampByTileMap(position);
	}

	//if (position.y > 0.f)
	//{
	//	isGrounded = true;
	//	velocity.y = 0.f;
	//}
	SetPosition(position);

	if (h != 0.f)
	{
		SetFlipX(h < 0);
	}

	if (animator.GetCurrentClipId() == "animations/Idle.csv")
	{
		if (h != 0.f)
		{
			animator.Play("animations/Run.csv");
		}
	}
	else if (animator.GetCurrentClipId() == "animations/Run.csv")
	{
		if (h == 0.f)
		{
			animator.Play("animations/Idle.csv");
		}
	}
	else if (animator.GetCurrentClipId() == "animations/Jump.csv" && isGrounded)
	{
		animator.Play("animations/Idle.csv");
	}

}
