#pragma once
#include "SpriteGo.h"
#include "Animator.h"
#include "SceneAnimationTool.h"

class PreviewCharacter : public SpriteGo
{
protected:
	PreviewCharacter(const PreviewCharacter&) = delete;
	PreviewCharacter(PreviewCharacter&&) = delete;
	PreviewCharacter& operator=(const PreviewCharacter&) = delete;
	PreviewCharacter& operator=(PreviewCharacter&&) = delete;

	SceneAnimationTool* sceneAnimationTool = nullptr;
	Animator animator;

	sf::RectangleShape shape;
	sf::Vector2f windowSize;
	sf::Vector2f previewLeftTop;

public:
	PreviewCharacter(const std::string& name = "previewcharacter");
	~PreviewCharacter() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	Animator& GetAnimator() { return animator; }
	void AdjustScale();
};