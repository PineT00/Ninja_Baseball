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
	// 이 부분을 에디터에서 만든 애니메이션으로 실행 필요
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

	// 가로와 세로 스케일 비율을 계산합니다.
	float scaleX = desiredWidth / textureSize.x;
	float scaleY = desiredHeight / textureSize.y;

	// 더 큰 스케일 비율을 선택하여 두 축 모두에 적용합니다.
	// 이렇게 하면 원본 비율을 유지하면서 텍스쳐를 조정할 수 있습니다.
	float scale = std::max(scaleX, scaleY);

	sprite.setScale(scale, scale);
}
