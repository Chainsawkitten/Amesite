#include "GameSettings.hpp"

#include <Util/FileSystem.hpp>

GameSettings::GameSettings() {
    mIni.SetUnicode(true);
    mIni.LoadFile(FileSystem::SavePath("Modership", "GameSettings.ini").c_str());
    
    AddBoolSetting("Logging", "Debug", "Logging", false);
    AddBoolSetting("Debug Context", "Debug", "Debug Context", false);
    AddBoolSetting("Show Frame Times", "Debug", "Show Frame Times", false);
    
    AddLongSetting("Screen Width", "Graphics", "Width", 800);
	AddLongSetting("Screen Height", "Graphics", "Height", 600);
    AddBoolSetting("Fullscreen", "Graphics", "Fullscreen", false);
    AddBoolSetting("Borderless", "Graphics", "Borderless Window", false);
    AddLongSetting("Target FPS", "Graphics", "Target FPS", 60);
    AddBoolSetting("VSync", "Graphics", "VSync", false);

    AddBoolSetting("FXAA", "Graphics", "FXAA", false);
    AddDoubleSetting("Gamma", "Graphics", "Brightness", 2.2);
       
    AddDoubleSetting("Aim Deadzone", "Controller", "Aim Deadzone", 0.5);
    AddDoubleSetting("Move Deadzone", "Controller", "Move Deadzone", 0.5);
}

GameSettings& GameSettings::GetInstance() {
    static GameSettings settings;
    
    return settings;
}

void GameSettings::Save() {
    mIni.SaveFile(FileSystem::SavePath("Modership", "GameSettings.ini").c_str());
}
