#pragma once
#include "Singleton.h"
class SoundManager : public Singleton<SoundManager>
{
	friend class Singleton<SoundManager>;
private :
	SoundManager();
	virtual ~SoundManager();

	sf::Sound bgm[2];
	int frontBgmIndex = 0;
	int backBgmIndex = 1; 

	std::list<sf::Sound*> playingSFX;
	std::list<sf::Sound*> waitingSFX;

	bool isFading = false;

	float sfxVolume = 100.f;
	float bgmVolume = 70.f;
	float crossFadeDuration = 10.f;
	float crossFadeTimer = 0.f;

	float fadeSpeed = 1.f;
	float fadeLimit = 0.05f;
	float fadeTimer = 0.f;
	float fadeDuration = 3.f;

public :
	void Init(int totalChannels = 128);
	void Release();
	void Update(float dt);

	void PlayBgm(const std::string& id, bool crossFade = true);
	void StopBgm();
	void SetBGMVolume(float volume) { this->bgmVolume = volume; }

	void PlaySfx(const std::string& id, bool loop = false);
	void PlaySfx(const sf::SoundBuffer& buffer, bool loop = false);
	void StopAllSfx();
	void SetVolume(float volume) { this->sfxVolume = volume; }
};
#define SOUND_MANAGER (Singleton<SoundManager>::Instance())