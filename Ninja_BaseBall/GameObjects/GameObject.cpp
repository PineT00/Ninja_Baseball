#include "pch.h"
#include "GameObject.h"

bool GameObject::CompareDrawOrder(const GameObject& lhs, const GameObject& rhs)
{
	if (lhs.sortLayer != rhs.sortLayer)
	{
		return lhs.sortLayer < rhs.sortLayer;
	}
	return lhs.sortOrder < rhs.sortOrder;
}

bool GameObject::CompareDrawOrder(const GameObject* lhs, const GameObject* rhs)
{
	if (lhs->sortLayer != rhs->sortLayer)
	{
		return lhs->sortLayer < rhs->sortLayer;
	}
	return lhs->sortOrder < rhs->sortOrder;
}

GameObject::GameObject(const std::string& name)
	: name(name)
{
}

GameObject::~GameObject()
{
}

void GameObject::SetPosition(float x, float y)
{
}

void GameObject::SetOrigin(Origins preset)
{
	if (preset == Origins::CUSTOM);
	originPreset = preset;
	origin = { 0.f, 0.f };
}

void GameObject::Init()
{
}

void GameObject::Release()
{
}

void GameObject::Reset()
{
}

void GameObject::Update(float dt)
{
}

void GameObject::LateUpdate(float dt)
{
}

void GameObject::FixedUpdate(float dt)
{
}

void GameObject::Draw(sf::RenderWindow& window)
{
}

void GameObject::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
}

bool GameObject::operator>(const GameObject& rhs) const
{
	if (sortLayer != rhs.sortLayer)
	{
		return sortLayer > rhs.sortLayer;
	}
	return sortOrder > rhs.sortOrder;
}

bool GameObject::operator<(const GameObject& rhs) const
{
	if (sortLayer != rhs.sortLayer)
	{
		return sortLayer < rhs.sortLayer;
	}
	return sortOrder < rhs.sortOrder;
}
