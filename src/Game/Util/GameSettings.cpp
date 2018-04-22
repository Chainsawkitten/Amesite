#include "GameSettings.hpp"

#include <Engine/Util/FileSystem.hpp>

GameSettings::GameSettings() {
    mIni.SetUnicode(true);
    mIni.LoadFile(FileSystem::SavePath("Amesite", "GameSettings.ini").c_str());
    
    AddBoolSetting("Logging", "Debug", "Logging", false);
    AddBoolSetting("Debug Context", "Debug", "Debug Context", false);
    AddBoolSetting("Show Notifications", "Debug", "Show Notification Messages", false);
    AddBoolSetting("Show Frame Times", "Debug", "Show Frame Times", false);
    AddBoolSetting("Show Light Volumes", "Debug", "Show Light Volumes", false);
    AddBoolSetting("Tiles", "Debug", "Visualize Tiles", false);
    
    AddLongSetting("Screen Width", "Graphics", "Width", 800);
    AddLongSetting("Screen Height", "Graphics", "Height", 600);
    AddBoolSetting("Fullscreen", "Graphics", "Fullscreen", false);
    AddBoolSetting("Borderless", "Graphics", "Borderless Window", false);
    AddLongSetting("Target FPS", "Graphics", "Target FPS", 60);
    AddBoolSetting("VSync", "Graphics", "VSync", false);
    AddBoolSetting("FXAA", "Graphics", "FXAA", false);
    AddBoolSetting("Reflections", "Graphics", "Water Reflections", true);
    AddBoolSetting("Refractions", "Graphics", "Water Refractions", true);
    AddDoubleSetting("Gamma", "Graphics", "Brightness", 2.2);
    AddBoolSetting("Dithering", "Graphics", "Dithering", true);
    
    AddDoubleSetting("Audio Volume", "Sound", "Volume", 1.0);
    
    AddDoubleSetting("Aim Deadzone", "Controller", "Aim Deadzone", 0.3);
    AddDoubleSetting("Move Deadzone", "Controller", "Move Deadzone", 0.3);
    
    AddBoolSetting("Show Splash Screen", "Game", "Show Splash Screen", true);
    AddBoolSetting("Player One Joystick Aim", "Game", "Player One Joystick Aim", false);
    AddBoolSetting("Two Players", "Game", "Two Players", true);
}

GameSettings& GameSettings::GetInstance() {
    static GameSettings settings;
    
    return settings;
}

void GameSettings::Save() {
    mIni.SaveFile(FileSystem::SavePath("Amesite", "GameSettings.ini").c_str());
}
