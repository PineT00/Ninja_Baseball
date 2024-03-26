#pragma once
#include "InputField.h"

enum class AnimationLoopType
{
	Single,
	Loop,
	PingPong,
};

struct AnimationFrame
{
	std::string textureId;
	sf::IntRect textureCoord;
	sf::Vector2f customPivot;
	Origins pivot = Origins::CUSTOM;

	AnimationFrame(const std::string& id, const sf::IntRect& coord, Origins origin, const sf::Vector2f& customPivot = {0.f, 0.f})
		: textureId(id), textureCoord(coord), pivot(origin)
	{
		if (pivot == Origins::CUSTOM)
		{
			this->customPivot = customPivot;
		}
	}


	const sf::Texture& GetTexture() const
	{
		return *TEXTURE_MANAGER.GetResource(textureId);
	}
};

struct AnimationClip
{
	std::string id;
	int fps = 30;
	AnimationLoopType loopType = AnimationLoopType::Single;

	std::vector<AnimationFrame> frames;

	int GetTotalFrame() const
	{
		return frames.size();
	}

	bool loadFromFile(const std::string& filePath);
};

struct AnimationEvent
{
	std::string clipId;
	int frame;
	std::function<void()> action;
};

class Animator
{
private:
	Animator(const Animator&) = delete;
	Animator(Animator&&) = delete;
	Animator& operator=(const Animator&) = delete;
	Animator& operator=(Animator&&) = delete;

protected:
	AnimationClip* currentClip = new AnimationClip();

	std::queue<std::string> queue;
	std::list<AnimationEvent> eventList;
	std::unordered_map<std::string, std::function<void()>> completeEvent;

	bool isCompleteClip = false;

	sf::Sprite* target = nullptr;

	float speed = 1.f;
	float clipDuration = 0.f;
	float accumTime = 0.f;

	int totalFrame = 0;
	int currentFrame = -1;
	int addFrame = 1;

	bool isPlaying = false;

public:
	Animator();
	virtual ~Animator();

	void Update(float dt);
	void Play(const std::string& clipId, bool clearQueue = true);
	void Play(std::vector<sf::FloatRect>& selectedAreas, std::vector<Origins>& selectedAreasPivot, 
		std::vector<sf::Vector2f>& customPivot, InputField* inputfieldFPS, AnimationLoopType& loopType, const std::wstring& atlasPath, bool clearQueue = true);
	void PlayQueue(const std::string& clipId);

	void Play(const std::string& clipId, int startFrame, bool clearQueue = true); //잔상효과용. 지정프레임부터 시작

	void Stop();

	const std::string& GetCurrentClipId() { return this->currentClip->id; }
	AnimationClip* GetCurrentClip() const { return currentClip; }

	const int GetCurrentClipFrame() { return this->currentFrame; }
	
	sf::Sprite* GetTarget() const { return this->target; }
	void SetTarget(sf::Sprite* target) { this->target = target; }

	float GetSpeed() const { return this->speed; }
	void SetSpeed(float speed) { this->speed = speed; }
	void SetFrame(const AnimationFrame& frame);

	bool IsPlaying() const { return this->isPlaying; }
	void AddEvent(const std::string& clipId, int frame, std::function<void()> action);
	void ClearEvent();
	void ClearFrames();


	/*
	1. 프레임이 하나인 경우 실행할 이벤트를 처리
	2. 애니메이션 클립의 Loop Type이 Single 경우 마지막 프레임에 실행할 이벤트를 처리
	*/

	void AddCompleteFrameEvent(const std::string& clipId, int frames, std::function<void()> action);
	void ClearCompleteEvent();
};