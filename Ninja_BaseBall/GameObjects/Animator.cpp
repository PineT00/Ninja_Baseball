#include "pch.h"
#include "Animator.h"
#include "rapidcsv.h"

Animator::Animator()
{
}

Animator::~Animator()
{
}

void Animator::Update(float dt)
{
	if (!isPlaying)
		return;

	accumTime += dt * speed;
	if (accumTime < clipDuration)
		return;

	accumTime = 0.f;
	currentFrame += addFrame;

	if (currentFrame == totalFrame)
	{
		if (!queue.empty())
		{
			std::string id = queue.front();
			queue.pop();
			Play(id, false);
			return;
		}

		switch (currentClip->loopType)
		{
		case AnimationLoopType::Single:
			currentFrame = totalFrame - 1;
			break;
		case AnimationLoopType::Loop:
			currentFrame = 0;
			break;
		case AnimationLoopType::PingPong:
			if (addFrame > 0)
			{
				currentFrame = totalFrame - 2;
				addFrame = -1;
				totalFrame = -1;
			}
			else
			{
				currentFrame = 1;
				addFrame = 1;
				totalFrame = currentClip->GetTotalFrame();
			}
			break;
		}
	}

	for (auto& event : eventList)
	{
		if (currentClip->id == event.clipId && currentFrame == event.frame)
		{
			if (event.action != nullptr)
			{
				event.action();
			}
		}
	}

	SetFrame(currentClip->frames[currentFrame]);
}

void Animator::Play(const std::string& clipId, bool clearQueue)
{
	if (clearQueue)
	{
		while (!queue.empty())
		{
			queue.pop();
		}
	}

	addFrame = 1;
	isPlaying = true;
	accumTime = 0.f;
	currentClip = ANIMATION_CLIP_MANAGER.GetResource(clipId);
	currentFrame = 0;
	totalFrame = currentClip->GetTotalFrame();
	clipDuration = 1.f / currentClip->fps;
	SetFrame(currentClip->frames[currentFrame]);
}

// TODO : 미리보기 작업중
void Animator::Play(std::vector<sf::FloatRect>& selectedAreas, 
	std::vector<Origins>& selectedAreasPivot, std::vector<sf::Vector2f>& customPivot, InputField* inputfieldFPS,
	 AnimationLoopType& loopType, const std::wstring& atlasPath, bool clearQueue)
{
	if (clearQueue)
	{
		while (!queue.empty())
		{
			queue.pop();
		}
	}

	addFrame = 1;
	isPlaying = true;
	accumTime = 0.f;
	currentClip->fps = std::stoi(inputfieldFPS->GetText());

	for (int i = 0; i < selectedAreas.size(); ++i)
	{
		currentClip->frames.push_back(
			{
			Utils::MyString::WideStringToString(atlasPath),
			{
				(int)selectedAreas[i].left,
				(int)selectedAreas[i].top,
				(int)selectedAreas[i].width,
				(int)selectedAreas[i].height,
			},
			selectedAreasPivot[i],
			customPivot[i]
			});
	}
	currentClip->id = Utils::MyString::WideStringToString(atlasPath);
	currentClip->loopType = loopType;
	currentFrame = 0;
	totalFrame = selectedAreas.size();
	clipDuration = 1.f / currentClip->fps;
	SetFrame(currentClip->frames[currentFrame]);
}

void Animator::PlayQueue(const std::string& clipId)
{
	queue.push(clipId);
}

void Animator::Play(const std::string& clipId, int startFrame, bool clearQueue)
{
	if (clearQueue)
	{
		while (!queue.empty())
		{
			queue.pop();
		}
	}

	addFrame = 1;
	isPlaying = true;
	accumTime = 0.f;
	currentClip = ANIMATION_CLIP_MANAGER.GetResource(clipId);
	currentFrame = startFrame;
	totalFrame = currentClip->GetTotalFrame();
	clipDuration = 1.f / currentClip->fps;
	SetFrame(currentClip->frames[currentFrame]);
}

void Animator::Stop()
{
	isPlaying = false;
}

void Animator::SetFrame(const AnimationFrame& frame)
{
	if (frame.pivot == Origins::CUSTOM)
	{
		target->setOrigin(frame.customPivot);
	}
	else
	{
		Utils::Origin::SetOrigin(*target, frame.pivot);
	}
	target->setTexture(frame.GetTexture());
	target->setTextureRect(frame.textureCoord);
}

void Animator::AddEvent(const std::string& clipId, int frame, std::function<void()> action)
{
	eventList.push_back({ clipId, frame, action });
}

void Animator::ClearEvent()
{
	eventList.clear();
}

void Animator::ClearFrames()
{
	if (currentClip->frames.empty()) return;
	currentClip->frames.clear();
}

bool AnimationClip::loadFromFile(const std::string& filePath)
{
	rapidcsv::Document doc(filePath);
	id = filePath;
	fps = doc.GetCell<int>(1, 0);
	loopType = static_cast<AnimationLoopType>(doc.GetCell<int>(2, 0));

	for (int i = 3; i < doc.GetRowCount(); ++i)
	{
		auto row = doc.GetRow<std::string>(i);
		frames.push_back(
			{ row[0],
			{
				std::stoi(row[1]),
				std::stoi(row[2]),
				std::stoi(row[3]),
				std::stoi(row[4]),
			},
			((Origins)std::stoi(row[5])),
			{std::stof(row[6]), std::stof(row[7])}
			});
	}

	return true;
}
