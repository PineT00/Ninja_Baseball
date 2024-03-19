#include "pch.h"
#include "Button.h"
#include "TextGo.h"
#include "SceneAnimationTool.h"

Button::Button(ButtonIdentifier identifier, const std::string& name)
	: SpriteGo(name), buttonIdentifier(identifier)
{
}

void Button::Init()
{	
	SpriteGo::Init();
}

void Button::Reset()
{
	sceneAnimationTool = dynamic_cast<SceneAnimationTool*>(SCENE_MANAGER.GetScene(SceneIDs::SceneAnimationTool));
}

void Button::Update(float dt)
{
	SpriteGo::Update(dt);
	SetButtonColor(buttonColor);

	if (InputManager::GetMouseButtonDown(sf::Mouse::Left))
	{
		if (GetGlobalBounds().contains(sceneAnimationTool->ScreenToUi((sf::Vector2i)InputManager::GetMousePos())))
		{
			isButtonPressed = true;
			SetButtonColor(buttonColorPressed);
		}
	}
	else if (InputManager::GetMouseButtonUp(sf::Mouse::Left))
	{
		if (GetGlobalBounds().contains(sceneAnimationTool->ScreenToUi((sf::Vector2i)InputManager::GetMousePos())))
		{
			if (isButtonPressed)
			{
				ExecuteButtonAction(buttonIdentifier);
				isButtonPressed = false;
			}
			SetButtonColor(buttonColor);
		}
	}
	else
	{
		if (GetGlobalBounds().contains(sceneAnimationTool->ScreenToUi((sf::Vector2i)InputManager::GetMousePos())))
		{
			SetButtonColor(buttonColorFocused);
		}
		else
		{
			SetButtonColor(buttonColor); 
		}
	}
}

void Button::ExecuteButtonAction(ButtonIdentifier id)
{
	switch (id)
	{
	case ButtonIdentifier::loadAtlas:
		OpenFileDialog(stringValue);
		break;
	case ButtonIdentifier::save :
		SaveSelectedAreasWithDialog();
		break;
	}
}

void Button::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	window.draw(shape);
	window.draw(buttonText);
}

void Button::SetOrigin(Origins preset)
{
	if (preset == Origins::CUSTOM)
	{
		preset == Origins::TL;
	}

	originPreset = preset;
	Utils::Origin::SetOrigin(shape, originPreset);
}

void Button::SetOrigin(const sf::Vector2f& origin)
{
	originPreset = Origins::CUSTOM;
	this->origin = origin;
	shape.setOrigin(this->origin);
}

void Button::SetButton(sf::Vector2f size, sf::Vector2f position, sf::Color color, Origins origin)
{
	shape.setSize(size);
	shape.setPosition(position);
	SetButtonColor(color);
	SetOrigin(origin);

}

void Button::SetButtonText(const sf::Font& font, const std::string& label, size_t labelSize, sf::Color labelColor, sf::Vector2f position, Origins origin)
{
	buttonText.setFont(font);
	buttonText.setString(label);
	buttonText.setCharacterSize(labelSize);
	buttonText.setFillColor(labelColor);
	buttonText.setPosition(position);
	Utils::Origin::SetOrigin(buttonText, origin);
}

void Button::SetButtonColor(sf::Color color)
{
	shape.setFillColor(color);
	SetButtonColorDefault(color);
	SetButtonColorFocused(color);
	SetButtonColorPressed(color);
}

void Button::SetButtonColorDefault(sf::Color color)
{
	color.a = 255.f;
	buttonColorDefault = color;
}

void Button::SetButtonColorFocused(sf::Color color)
{
	color.a = 200.f;
	buttonColorFocused = color;
}

void Button::SetButtonColorPressed(sf::Color color)
{
	color.a = 127.f;
	buttonColorPressed = color;
}

std::wstring Button::OpenFileDialog(std::wstring& filePath)
{
	wchar_t filename[MAX_PATH] = L"";

	OPENFILENAMEW ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAMEW);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = L"Image Files\0*.png;*.jpg;*.jpeg\0All Files\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"";

	if (GetOpenFileNameW(&ofn)) {
		filePath = filename;

		sceneAnimationTool->SetIsAtlasPath(false);
		sceneAnimationTool->SetAtlasPath(filePath);
		return filePath;
	}
	return L"";
}

void Button::SaveSelectedAreasWithDialog()
{
	OPENFILENAME ofn;       // 공통 대화 상자 구조체
	wchar_t  szFileName[MAX_PATH] = L""; // 파일 이름을 저장할 배열

	// OPENFILENAME 구조체 초기화
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // 현재 창의 핸들, NULL이면 부모 창 없음
	ofn.lpstrFilter = L"CSV Files (*.csv)\0*.csv\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = L"csv";

	// 파일 저장 대화 상자 표시
	if (GetSaveFileName(&ofn))
	{
		std::ofstream outFile(szFileName); // 사용자가 선택한 이름으로 파일 열기

		if (!outFile.is_open())
		{
			MessageBox(NULL, L"File could not be opened.", L"Error", MB_OK);
			return;
		}

		outFile << "Left,Top,Width,Height\n";

		for (const auto& area : sceneAnimationTool->GetSelectedAreas())
		{
			outFile << area.left << ","
				<< area.top << ","
				<< area.width << ","
				<< area.height << "\n";
		}

		outFile.close(); // 파일 닫기
	}
}

sf::FloatRect Button::GetLocalBounds()
{
	return shape.getLocalBounds();
}

sf::FloatRect Button::GetGlobalBounds()
{
	return shape.getGlobalBounds();
}