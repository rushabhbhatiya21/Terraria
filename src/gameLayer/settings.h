#pragma once

#include <algorithm>

struct Settings
{
	float musicVolume = 0.75f;
	float masterVolume = 0.75f;
	float soundsVolume = 0.75f;

	void sanitize()
	{
		musicVolume = std::clamp(musicVolume, 0.f, 1.f);
		masterVolume = std::clamp(masterVolume, 0.f, 1.f);
		soundsVolume = std::clamp(soundsVolume, 0.f, 1.f);
	}

	bool operator==(const Settings& other) const
	{
		return musicVolume == other.musicVolume &&
			masterVolume == other.masterVolume &&
			soundsVolume == other.soundsVolume;
	}

	bool operator!=(const Settings& other) const
	{
		return !(*this == other);
	}
};

Settings& getSettings();

void saveSettings();
void loadSettings();
void updateSettings();