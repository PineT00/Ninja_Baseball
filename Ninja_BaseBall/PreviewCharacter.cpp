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

	previewLeftTop = { windowSize.x * 0.05f, windowSize.y * 0.7f };
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
}

void PreviewCharacter::Draw(sf::RenderWindow& window)
{
	const sf::View& saveView = window.getView();
	window.setView(sceneAnimationTool->GetUiView());

	SpriteGo::Draw(window);

	window.setView(saveView);
}
