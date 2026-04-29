#include "settings.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>

#define SETTINGS_VERSION 1

Settings settings;
Settings settingsOld;

using Json = nlohmann::json;

Settings& getSettings()
{
	return settings;
}

void saveSettings()
{
	Json j;

	j["version"] = SETTINGS_VERSION;
	j["musicVolume"] = settings.musicVolume;
	j["masterVolume"] = settings.masterVolume;
	j["soundsVolume"] = settings.soundsVolume;

	std::error_code error;
	std::filesystem::create_directory(RESOURCES_PATH "../settings", error);

	std::ofstream f(RESOURCES_PATH "../settings/settings.txt");
	f << j.dump(2);
	f.close();
}

void loadSettings()
{
	// reset all settings first
	// its possible for some data to be missing
	settings = {};

	std::ifstream f(RESOURCES_PATH "../settings/settings.txt");
	if (!f.is_open()) { return; }

	Json j;
	j = Json::parse(f, nullptr, /*allow exceptions=*/false);

	if (j["musicVolume"].is_number())
		settings.musicVolume = j["musicVolume"].get<float>();

	if (j["masterVolume"].is_number())
		settings.masterVolume = j["masterVolume"].get<float>();

	if (j["soundsVolume"].is_number())
		settings.soundsVolume = j["soundsVolume"].get<float>();

	settings.sanitize();

}

void updateSettings()
{
	if (settings != settingsOld)
	{
		saveSettings();
	}
	settingsOld = settings;
}