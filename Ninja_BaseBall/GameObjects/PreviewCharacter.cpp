#include "pch.h"
#include "PreviewCharacter.h"

PreviewCharacter::PreviewCharacter(const std::string& name)
	: SpriteGo(name)
{
	windowSize = (sf::Vector2f)FRAMEWORK.GetWindowSize();
}

void PreviewCharacter::Init()
{
	SpriteGo::Init();

	animator.SetTarget(&sprite);
	sceneAnimationTool = dynamic_cast<SceneAnimationTool*>(SCENE_MANAGER.GetScene(SceneIDs::SceneAnimationTool));

	previewLeftTop = { windowSize.x * 0.1f, windowSize.y * 0.75f };
}

void PreviewCharacter::Reset()
{
	shape.setPosition(previewLeftTop);
	SetPosition(previewLeftTop);
	// �� �κ��� �����Ϳ��� ���� �ִϸ��̼����� ���� �ʿ�
	SetOrigin(Origins::MC);
}

void PreviewCharacter::Update(float dt)
{
	SpriteGo::Update(dt);
	animator.Update(dt);
	if (sprite.getTexture() != nullptr)
	{
		AdjustScale();
	}
}

void PreviewCharacter::Draw(sf::RenderWindow& window)
{
	const sf::View& saveView = window.getView();
	window.setView(sceneAnimationTool->GetUiView());

	SpriteGo::Draw(window);

	window.setView(saveView);
}

void PreviewCharacter::AdjustScale()
{
	sf::Vector2u textureSize = sprite.getTexture()->getSize();

	float desiredWidth = 150.0f;
	float desiredHeight = 150.0f;

	// ���ο� ���� ������ ������ ����մϴ�.
	float scaleX = desiredWidth / textureSize.x;
	float scaleY = desiredHeight / textureSize.y;

	// �� ū ������ ������ �����Ͽ� �� �� ��ο� �����մϴ�.
	// �̷��� �ϸ� ���� ������ �����ϸ鼭 �ؽ��ĸ� ������ �� �ֽ��ϴ�.
	float scale = std::max(scaleX, scaleY);

	sprite.setScale(scale, scale);
}
