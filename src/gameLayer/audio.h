#pragma once

namespace Audio
{
	void init();

	void loadAllMusicAndSounds();

	void playMusic(int index);

	void stopAllMusic();

	void update();

	void playSound(int sound, float volume = 1);
	void playRandomInRange(int first, int last, float volume = 1);

	void playUiButton(bool backButton = false);
	void playUiSlider(float volume = 0.45f);

	void playRandomHit(float volume = 1);
	void playRandomImpact(float volume = 1);
	void playRandomFinalHit(float volume = 1);
	void playRandomStone(float volume = 1);
	void playRandomWood(float volume = 1);

	void playBlockPlaceForType(int itemType, float volume = 1);
	void playBlockBreakForType(int itemType, float volume = 1);

	bool isMusicPlaying();

	enum Sounds
	{
		noneSound = 0,
		placeBlock,
		breakBlock,

		uiButtonPress,
		uiBackButton,
		uiCheckOn,
		uiCheckOff,
		uiSlider,

		hit1,
		hit2,
		hit3,

		impact1,
		impact2,
		impact3,

		finalHit1,
		finalHit2,
		finalHit3,

		stone1,
		stone2,
		stone3,
		stone4,
		stone5,
		stone6,
		stone7,

		wood1,
		wood2,
		wood3,
		wood4,
		wood5,
		wood6,
		wood7,

		SOUNDS_COUNT
	};

	enum Musics
	{
		noneMusic = 0,
		musicForest,
		musicDesert,
		musicSnow,
		musicCave,

		MUSICS_COUNT
	};
}