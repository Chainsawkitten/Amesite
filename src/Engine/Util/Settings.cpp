#include "Settings.hpp"

using namespace std;

bool Settings::GetBool(const string& settingName) {
    BoolSetting setting = mBoolSettings[settingName];
    
    return mIni.GetBoolValue(setting.section.c_str(), setting.key.c_str(), setting.defaultValue);
}

void Settings::SetBool(const string& settingName, bool value) {
    BoolSetting setting = mBoolSettings[settingName];
    
    mIni.SetBoolValue(setting.section.c_str(), setting.key.c_str(), value);
}

long Settings::GetLong(const string& settingName) {
    LongSetting setting = mLongSettings[settingName];
    
    return mIni.GetLongValue(setting.section.c_str(), setting.key.c_str(), setting.defaultValue);
}

void Settings::SetLong(const string& settingName, long value) {
    LongSetting setting = mLongSettings[settingName];
    
    mIni.SetLongValue(setting.section.c_str(), setting.key.c_str(), value);
}

double Settings::GetDouble(const string& settingName) {
    DoubleSetting setting = mDoubleSettings[settingName];
    
    return mIni.GetDoubleValue(setting.section.c_str(), setting.key.c_str(), setting.defaultValue);
}

void Settings::SetDouble(const string& settingName, double value) {
    DoubleSetting setting = mDoubleSettings[settingName];
    
    mIni.SetDoubleValue(setting.section.c_str(), setting.key.c_str(), value);
}

string Settings::GetString(const string& settingName) {
    StringSetting setting = mStringSettings[settingName];
    
    return mIni.GetValue(setting.section.c_str(), setting.key.c_str(), setting.defaultValue.c_str());
}

void Settings::SetString(const string& settingName, const string& value) {
    StringSetting setting = mStringSettings[settingName];
    
    mIni.SetValue(setting.section.c_str(), setting.key.c_str(), value.c_str());
}

void Settings::AddBoolSetting(const string& settingName, const string& section, const string& key, bool defaultValue) {
    BoolSetting setting;
    setting.section = section;
    setting.key = key;
    setting.defaultValue = defaultValue;
    
    mBoolSettings[settingName] = setting;
    
    bool value = mIni.GetBoolValue(section.c_str(), key.c_str(), defaultValue);
    mIni.SetBoolValue(section.c_str(), key.c_str(), value);
}

void Settings::AddLongSetting(const string& settingName, const string& section, const string& key, long defaultValue) {
    LongSetting setting;
    setting.section = section;
    setting.key = key;
    setting.defaultValue = defaultValue;
    
    mLongSettings[settingName] = setting;
    
    long value = mIni.GetLongValue(section.c_str(), key.c_str(), defaultValue);
    mIni.SetLongValue(section.c_str(), key.c_str(), value);
}

void Settings::AddDoubleSetting(const string& settingName, const string& section, const string& key, double defaultValue) {
    DoubleSetting setting;
    setting.section = section;
    setting.key = key;
    setting.defaultValue = defaultValue;
    
    mDoubleSettings[settingName] = setting;
    
    double value = mIni.GetDoubleValue(section.c_str(), key.c_str(), defaultValue);
    mIni.SetDoubleValue(section.c_str(), key.c_str(), value);
}

void Settings::AddStringSetting(const string& settingName, const string& section, const string& key, const string& defaultValue) {
    StringSetting setting;
    setting.section = section;
    setting.key = key;
    setting.defaultValue = defaultValue;
    
    mStringSettings[settingName] = setting;
    
    const char* value = mIni.GetValue(section.c_str(), key.c_str(), defaultValue.c_str());
    mIni.SetValue(section.c_str(), key.c_str(), value);
}
