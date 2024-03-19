#pragma once

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
	Origins pivot = Origins::CUSTOM;

	AnimationFrame(const std::string& id, const sf::IntRect& coord)//, Origins origin)
		: textureId(id), textureCoord(coord) //pivot(origin)
	{
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
	AnimationClip* currentClip = nullptr;

	std::queue<std::string> queue;
	std::list<AnimationEvent> eventList;

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
	void PlayQueue(const std::string& clipId);
	void Stop();

	const std::string& GetCurrentClipId() { return this->currentClip->id; }

	sf::Sprite* GetTarget() const { return this->target; }
	void SetTarget(sf::Sprite* target) { this->target = target; }

	float GetSpeed() const { return this->speed; }
	void SetSpeed(float speed) { this->speed = speed; }

	void SetFrame(const AnimationFrame& frame);

	bool IsPlaying() const { return this->isPlaying; }

	void AddEvent(const std::string& clipId, int frame, std::function<void()> action);
	void ClearEvent();
};