#include "pch.h"
#include "InputField.h"

InputField::InputField(const std::string& name)
	: TextGo(name)
{
}

void InputField::Init()
{
	TextGo::Init();
}

void InputField::Reset()
{
	TextGo::Reset();
}

void InputField::Update(float dt)
{
	TextGo::Update(dt);
}
