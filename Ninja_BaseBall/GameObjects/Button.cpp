#include "pch.h"
#include "Button.h"
#include "TextGo.h"
#include "SceneAnimationTool.h"
#include "InputField.h"
#include "PreviewCharacter.h"

std::wstring Button::emptyWstring = std::wstring(L"");

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
	case ButtonIdentifier::loadcsv :
		LoadCsv();
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
	wchar_t currentPath[1024];
	GetCurrentDirectory(1024, currentPath);

	wchar_t filename[MAX_PATH] = L"";

	OPENFILENAMEW ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAMEW);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = L"Image Files\0*.png;*.jpg;*.jpeg;*.bmp\0All Files\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"";

	std::vector<sf::FloatRect>& selectedAreas = sceneAnimationTool->GetSelectedAreas();
	std::vector<Origins>& selectedAreasPivot = sceneAnimationTool->GetSelectedAreasPivot();
	std::vector<sf::Vector2f>& customPivot = sceneAnimationTool->GetCustomPivot();

	selectedAreas.clear();
	selectedAreasPivot.clear();
	customPivot.clear();
	sceneAnimationTool->SetAtlasPath(emptyWstring);
	sceneAnimationTool->SetIsAtlasPath(false);

	if (GetOpenFileNameW(&ofn)) {
		filePath = filename;

		sceneAnimationTool->SetIsAtlasPath(false);
		sceneAnimationTool->SetAtlasPath(filePath);

		SetCurrentDirectory(currentPath);
		return filePath;
	}

	SetCurrentDirectory(currentPath);
	return L"";
}

void Button::SaveSelectedAreasWithDialog()
{
	wchar_t currentPath[1024];
	GetCurrentDirectory(1024, currentPath);

	std::vector<sf::FloatRect>& area = sceneAnimationTool->GetSelectedAreas();
	std::vector<Origins>& pivotList = sceneAnimationTool->GetSelectedAreasPivot();
	std::vector<sf::Vector2f>& customPivots = sceneAnimationTool->GetCustomPivot();
	AnimationLoopType& loopType = sceneAnimationTool->GetSelectedLoopType();
	
	if (area.empty())
	{
		
		return;
	}
	if (area.size() != pivotList.size()) return;

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
		
		SetCurrentDirectory(currentPath);

		std::string sFp = Utils::MyString::WideStringToString(sceneAnimationTool->GetAtlasPath());

		for (int i = 0; i < sFp.size(); ++i)
		{
			if (sFp[i] == '\\')
			{
				sFp[i] = '\/';
			}
		}

		std::string fp = sFp.substr(sFp.find("graphics"), sFp.size());

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
	std::vector<sf::FloatRect> selectedAreas = sceneAnimationTool->GetSelectedAreas();

	if (selectedAreas.empty()) return;

	std::vector<Origins>& pivotList = sceneAnimationTool->GetSelectedAreasPivot();
	std::vector<sf::Vector2f>& customPivotList = sceneAnimationTool->GetCustomPivot();
	Origins frameOrigin = (Origins)(std::stoi(name.substr(name.size() -1)));
	
	if (frameOrigin == Origins::CUSTOM)
	{
		sceneAnimationTool->SetIsCutomPivot(true);
	}
	else
	{
		customPivotList.push_back({ 0,0 });
		std::cout << "Selected Pivot : " << (int)frameOrigin << std::endl;
	}
	pivotList.push_back(frameOrigin);
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

void Button::LoadCsv()
{
	wchar_t currentPath[1024];
	GetCurrentDirectory(1024, currentPath);

	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH] = L"";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = L"CSV Files (*.csv)\0*.csv\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = L"csv";

	if (GetOpenFileName(&ofn)) {
		std::wifstream inFile(szFileName);
		if (!inFile.is_open()) {
			MessageBox(NULL, L"File could not be opened.", L"Error", MB_OK);
			return;
		}

		SetCurrentDirectory(currentPath);

		if (obj->GetAnimator().IsPlaying())
		{
			obj->GetAnimator().Stop();
		}
		
		std::wstring line;
		std::getline(inFile, line);
		std::getline(inFile, line);

		std::wistringstream iss(line);
		std::wstring id, fps, loopType;

		std::vector<sf::FloatRect>& selectedAreas = sceneAnimationTool->GetSelectedAreas();
		std::vector<Origins>& selectedAreasPivot = sceneAnimationTool->GetSelectedAreasPivot();
		std::vector<sf::Vector2f>& customPivot = sceneAnimationTool->GetCustomPivot();

		selectedAreas.clear();
		selectedAreasPivot.clear();
		customPivot.clear();
		sceneAnimationTool->SetAtlasPath(emptyWstring);
		sceneAnimationTool->SetIsAtlasPath(false);

		std::getline(iss, id, L',');
		std::getline(iss, fps, L',');
		std::getline(iss, loopType, L',');

		sceneAnimationTool->SetFPS(fps);
		sceneAnimationTool->SetLoopType((AnimationLoopType)std::stoi(loopType));

		std::getline(inFile, line);
		std::getline(inFile, line);

		while (std::getline(inFile, line)) 
		{
			if (line.empty()) continue; 

			std::wistringstream textureStream(line);
			std::wstring texturePath, left, top, width, height, origin, customX, customY;
			std::getline(textureStream, texturePath, L',');
			std::getline(textureStream, left, L',');
			std::getline(textureStream, top, L',');
			std::getline(textureStream, width, L',');
			std::getline(textureStream, height, L',');
			std::getline(textureStream, origin, L',');
			std::getline(textureStream, customX, L',');
			std::getline(textureStream, customY, L',');

			if (sceneAnimationTool->GetAtlasPath() == L"")
			{
				std::wstring formattedPath = texturePath;

				sceneAnimationTool->SetIsAtlasPath(false);
				sceneAnimationTool->SetAtlasPath(formattedPath);
			}
			selectedAreas.push_back({ std::stof(left),std::stof(top), std::stof(width), std::stof(height) });
			selectedAreasPivot.push_back((Origins)std::stoi(origin));
			customPivot.push_back({ std::stof(customX), std::stof(customY) });

			std::wcout << line << std::endl;
		}
		sceneAnimationTool->SetLoadedFromCsv(true);

		inFile.close(); 
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