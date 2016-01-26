#include "GameSettings.hpp"

#include <Util/FileSystem.hpp>

GameSettings::GameSettings() {
    mIni.SetUnicode(true);
    mIni.LoadFile(FileSystem::SavePath("Modership", "GameSettings.ini").c_str());
    
    AddBoolSetting("Logging", "Debug", "Logging", false);
    AddBoolSetting("Debug Context", "Debug", "Debug Context", false);
    
    AddLongSetting("Screen Width", "Graphics", "Width", 800);
	AddLongSetting("Screen Height", "Graphics", "Height", 600);
    AddBoolSetting("Fullscreen", "Graphics", "Fullscreen", false);
    AddBoolSetting("Borderless", "Graphics", "Borderless Window", false);
}

GameSettings& GameSettings::GetInstance() {
    static GameSettings settings;
    
    return settings;
}

void GameSettings::Save() {
    mIni.SaveFile(FileSystem::SavePath("Modership", "GameSettings.ini").c_str());
}
