#include "audio.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <raylib.h>
#include <asserts.h>
#include <settings.h>
#include <items/itemIds.h>
#include <randomStuff.h>

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
	std::ranlux24_base sfxRng(std::random_device{}());

	static bool isWoodLike(int itemType)
	{
		switch (itemType)
		{
		case Items::woodLog:
		case Items::woodPlank:
		case Items::platform:
		case Items::workBench:
		case Items::woodenChest:
		case Items::woodWall:
		case Items::plankedWall:
			return true;
		default:
			return false;
		}
	}

	static bool isStoneLike(int itemType)
	{
		switch (itemType)
		{
		case Items::stone:
		case Items::stoneBricks:
		case Items::sandStone:
		case Items::bricks:
		case Items::furnace:
		case Items::stoneWall:
		case Items::stoneBricksWall:
		case Items::brickWall:
			return true;
		default:
			return false;
		}
	}

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

		loadSound(RESOURCES_PATH "sounds/bonus/ButtonPress.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/BackButton.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/CheckBoxOn.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/CheckBoxOff.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/Slider.ogg");

		loadSound(RESOURCES_PATH "sounds/bonus/Hit1.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/Hit2.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/Hit3.ogg");

		loadSound(RESOURCES_PATH "sounds/bonus/HImpact1.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/HImpact2.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/HImpact3.ogg");

		loadSound(RESOURCES_PATH "sounds/bonus/FinalHit1.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/FinalHit2.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/FinalHit3.ogg");

		loadSound(RESOURCES_PATH "sounds/bonus/Stone1.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/Stone2.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/Stone3.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/Stone4.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/Stone5.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/Stone6.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/Stone7.ogg");

		loadSound(RESOURCES_PATH "sounds/bonus/Wood1.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/Wood2.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/Wood3.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/Wood4.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/Wood5.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/Wood6.ogg");
		loadSound(RESOURCES_PATH "sounds/bonus/Wood7.ogg");

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

		volume = std::clamp(volume, 0.f, 1.f);

		// less loud
		volume *= getSettings().masterVolume * getSettings().masterVolume;
		volume *= getSettings().soundsVolume * getSettings().soundsVolume;

		SetSoundVolume(allSounds[sound], volume);
		PlaySound(allSounds[sound]);
	}

	void playRandomInRange(int first, int last, float volume)
	{
		if (first > last) { return; }
		int pick = getRandomInt(sfxRng, first, last);
		playSound(pick, volume);
	}

	void playUiButton(bool backButton)
	{
		playSound(backButton ? uiBackButton : uiButtonPress, 0.7f);
	}

	void playUiSlider(float volume)
	{
		playSound(uiSlider, volume);
	}

	void playRandomHit(float volume)
	{
		playRandomInRange(hit1, hit3, volume);
	}

	void playRandomImpact(float volume)
	{
		playRandomInRange(impact1, impact3, volume);
	}

	void playRandomFinalHit(float volume)
	{
		playRandomInRange(finalHit1, finalHit3, volume);
	}

	void playRandomStone(float volume)
	{
		playRandomInRange(stone1, stone7, volume);
	}

	void playRandomWood(float volume)
	{
		playRandomInRange(wood1, wood7, volume);
	}

	void playBlockPlaceForType(int itemType, float volume)
	{
		if (isWoodLike(itemType))
		{
			playRandomWood(volume * 0.85f);
			return;
		}

		if (isStoneLike(itemType))
		{
			playRandomStone(volume * 0.9f);
			return;
		}

		playSound(placeBlock, volume);
	}

	void playBlockBreakForType(int itemType, float volume)
	{
		if (isWoodLike(itemType))
		{
			playRandomWood(volume);
			return;
		}

		if (isStoneLike(itemType))
		{
			playRandomStone(volume);
			return;
		}

		playSound(breakBlock, volume);
	}

	bool isMusicPlaying()
	{
		if (!currentMusicPlaying) { return false; }
		return IsAudioStreamPlaying(allMusic[currentMusicPlaying].stream);
	}
}