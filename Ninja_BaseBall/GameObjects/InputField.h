#pragma once
#include "TextGo.h"
class InputField : public TextGo
{
protected:
	InputField(const InputField&) = delete;
	InputField(InputField&&) = delete;
	InputField& operator=(const InputField&) = delete;
	InputField& operator=(InputField&&) = delete;

public:
	InputField(const std::string& name = "");
	~InputField() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
};