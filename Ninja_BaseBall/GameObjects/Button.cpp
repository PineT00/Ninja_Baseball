#include "pch.h"
#include "Button.h"
#include "TextGo.h"
#include "SceneAnimationTool.h"
#include "InputField.h"
#include "PreviewCharacter.h"

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
	obj = dynamic_cast<PreviewCharacter*>(sceneAnimationTool->FindGameObject("previewcharacter"));
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
	case ButtonIdentifier::pivot :
		SetFramePivot();
		break;
	case ButtonIdentifier::loop :
		SetLoopType();
		break;
	case ButtonIdentifier::play :
		PlayPreView(obj);
		break;
	case ButtonIdentifier::stop :
		StopPreView(obj);
		break;
	case ButtonIdentifier::autoslice :
		AutoSlice(intValues);
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

void Button::SetText(const std::string& label)
{
	buttonText.setString(label);
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
	OPENFILENAME ofn;
	wchar_t  szFileName[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = L"CSV Files (*.csv)\0*.csv\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = L"csv";

	if (GetSaveFileName(&ofn))
	{
		std::ofstream outFile(szFileName);

		if (!outFile.is_open())
		{
			MessageBox(NULL, L"File could not be opened.", L"Error", MB_OK);
			return;
		}

		std::string sFp = Utils::MyString::WideStringToString(sceneAnimationTool->GetAtlasPath());

		for (int i = 0; i < sFp.size(); ++i)
		{
			if (sFp[i] == '\\')
			{
				sFp[i] = '\/';
			}
		}

		std::string fp = sFp.substr(sFp.find("graphics"), sFp.size());
		std::vector<Origins>& pivotList = sceneAnimationTool->GetSelectedAreasPivot();
		std::vector<sf::FloatRect>& area = sceneAnimationTool->GetSelectedAreas();
		std::vector<sf::Vector2f>& customPivots = sceneAnimationTool->GetCustomPivot();
		AnimationLoopType& loopType = sceneAnimationTool->GetSelectedLoopType();

		outFile << "ID,FPS,LOOPTYPE(0 : Single, 1: Loop, 2 : PingPong)\n";
		outFile << "," << sceneAnimationTool->GetFPS()->GetText() << "," << (int)loopType <<"\n\n";
		outFile << "TEXTURE ID,LEFT,TOP,WIDTH,HEIGHT,ORIGIN,CUSTOMX,CUSTOMY\n";

		for (int i = 0; i < sceneAnimationTool->GetSelectedAreas().size(); ++i)
		{
			outFile << fp << ","
				<< area[i].left << ","
				<< area[i].top << ","
				<< area[i].width << ","
				<< area[i].height << ","
				<< (int)pivotList[i] << ","
				<< customPivots[i].x << ","
				<< customPivots[i].y << "\n";
		}

		std::cout << fp << ", 저장되었습니다." << std::endl;
		outFile.close(); // 파일 닫기
	}
}

void Button::SetFramePivot()
{
	std::vector<Origins>& pivotList = sceneAnimationTool->GetSelectedAreasPivot();
	std::vector<sf::Vector2f>& customPivotList = sceneAnimationTool->GetCustomPivot();
	Origins frameOrigin = (Origins)(std::stoi(name.substr(name.size() -1)));
	// Custom 피봇이 선택 되면 피봇을 선택할 수 있게 해야 함.
	
	if (frameOrigin == Origins::CUSTOM)
	{
		sceneAnimationTool->SetIsCutomPivot(true);
	}
	else
	{
		customPivotList.push_back({ 0,0 });
	}
	// 가장 마지막 SelectedArea의 위치를 받아 상대적인 좌표를 구해야 함
	//sf::Vector2f frameCustomOrigin = customPivotList[customPivotList.size() - 1]; // 에러 주의

	pivotList.push_back(frameOrigin);
	//customPivotList.push_back(frameCustomOrigin);
}

void Button::SetLoopType()
{
	AnimationLoopType& loopType = sceneAnimationTool->GetSelectedLoopType();
	loopType = (AnimationLoopType)(std::stoi(name.substr(name.size() - 1)));
}

void Button::PlayPreView(PreviewCharacter* obj)
{
	this->obj = obj;

	if (sceneAnimationTool->GetSelectedAreas().empty() || sceneAnimationTool->GetSelectedAreasPivot().empty() ||
		sceneAnimationTool->GetFPS() == nullptr) return;
	
	obj->GetAnimator().ClearFrames();

	obj->GetAnimator().Play(
		sceneAnimationTool->GetSelectedAreas(), 
		sceneAnimationTool->GetSelectedAreasPivot(),
		sceneAnimationTool->GetCustomPivot(),
		sceneAnimationTool->GetFPS(), 
		sceneAnimationTool->GetSelectedLoopType(), 
		sceneAnimationTool->GetAtlasPath(), false);
}

void Button::StopPreView(PreviewCharacter* obj)
{
	obj->GetAnimator().Stop();
}

void Button::AutoSlice(const std::vector<int> intValues)
{
	// 슬라이스 후 저장 준비
	
}

sf::FloatRect Button::GetLocalBounds()
{
	return shape.getLocalBounds();
}

sf::FloatRect Button::GetGlobalBounds()
{
	return shape.getGlobalBounds();
}