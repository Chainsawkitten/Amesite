#include "GameSettings.hpp"

//#include <Core/Util/FileSystem.hpp>

GameSettings::GameSettings() {
    mIni.SetUnicode(true);
    //ini.LoadFile(FileSystem::SavePath("Modership", "GameSettings.ini").c_str());
    mIni.LoadFile("Settings.ini");
    
    AddBoolSetting("Logging", "Debug", "Logging", false);
}

GameSettings& GameSettings::GetInstance() {
    static GameSettings settings;
    
    return settings;
}

void GameSettings::Save() {
    //ini.SaveFile(FileSystem::SavePath("Modership", "GameSettings.ini").c_str());
    mIni.SaveFile("Settings.ini");
}
