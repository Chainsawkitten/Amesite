#pragma once

#include <Util/Settings.hpp>

/// %Settings for the game.
/**
 * Available settings:
 * Name                | Description                         | Type   | Default Value
 * ------------------- | ----------------------------------- | ------ | -------------
 * Logging             | Output a log file.                  | bool   | false
 */
class GameSettings : public Settings {
    public:
        /// Get the instance of the class.
        /**
         * @return The %EditorSettings instance
         */
        static GameSettings& GetInstance();
        
        /// Save the settings to an INI file.
        void Save();
        
    private:
        GameSettings();
        
        GameSettings(GameSettings const&);
        void operator=(GameSettings const&);
};
