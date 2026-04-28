#include "audio.h"
#include <vector>
#include <cmath>
#include <raylib.h>
#include <raymath.h>
#include <asserts.h>
#include <settings.h>

namespace Audio
{
	void init()
	{
		InitAudioDevice();
		SetMasterVolume(0.9f);

		loadAllMusicAndSounds();
	}

	int currentMusicPlaying = 0;
	std::vector<Sound> allSounds;
	std::vector<Music> allMusic;

	void loadAllMusicAndSounds()
	{
		auto loadMusic = [&](const char* path)
			{
				Music m;
				m = LoadMusicStream(path);

				if (m.stream.buffer)
				{
					allMusic.push_back(m);
				}
				else
				{
					allMusic.push_back({}); // empty music
				}
			};

		allMusic.push_back({}); // empty music at index 0

		loadMusic(RESOURCES_PATH "music/forest.ogg");
		loadMusic(RESOURCES_PATH "music/desert.ogg");
		loadMusic(RESOURCES_PATH "music/snow.ogg");
		loadMusic(RESOURCES_PATH "music/cave.ogg");

		permaAssertComment(allMusic.size() == Musics::MUSICS_COUNT, "Forgot to add a song here!");

		auto loadSound = [&](const char* path)
			{
				Sound s;
				s = LoadSound(path);

				if (s.stream.buffer)
				{
					allSounds.push_back(s);
				}
				else
				{
					allSounds.push_back({}); // empty sound
				}
			};

		allSounds.push_back({}); // empty sound at index 0 to match enum

		loadSound(RESOURCES_PATH "sounds/place.ogg");
		loadSound(RESOURCES_PATH "sounds/break.ogg");

		permaAssertComment(allSounds.size() == Sounds::SOUNDS_COUNT, "Forgot to add a sound here!");
	}

	void playMusic(int index)
	{
		if (allMusic.size() <= index) { return; }
		if (currentMusicPlaying == index) { return; }

		StopMusicStream(allMusic[currentMusicPlaying]);

		allMusic[index].looping = true;
		PlayMusicStream(allMusic[index]);
		SetMusicVolume(
			allMusic[index],
			std::pow(getSettings().musicVolume * getSettings().musicVolume, 1.0f)
		);

		currentMusicPlaying = index;
	}

	void update()
	{
		if (!isMusicPlaying())
		{
			currentMusicPlaying = 0;
			return;
		}

		SetMusicVolume(
			allMusic[currentMusicPlaying],
			std::pow(getSettings().musicVolume * getSettings().musicVolume, 1.0f)
		);

		UpdateMusicStream(allMusic[currentMusicPlaying]);
	}

	void stopAllMusic()
	{
		StopMusicStream(allMusic[currentMusicPlaying]);
		currentMusicPlaying = 0;
	}

	void playSound(int sound, float volume)
	{
		if (sound <= Sounds::noneSound || sound >= Sounds::SOUNDS_COUNT) { return; }

		volume = Clamp(volume, 0, 1);

		// less loud
		volume *= getSettings().masterVolume * getSettings().masterVolume;
		volume *= getSettings().soundsVolume * getSettings().soundsVolume;

		SetSoundVolume(allSounds[sound], volume);
		PlaySound(allSounds[sound]);
	}

	bool isMusicPlaying()
	{
		if (!currentMusicPlaying) { return false; }
		return IsAudioStreamPlaying(allMusic[currentMusicPlaying].stream);
	}
}