#pragma once

#include <Util/Settings.hpp>

/// %Settings for the game.
/**
 * Available settings:
 * Name                | Description                         | Type   | Default Value
 * ------------------- | ----------------------------------- | ------ | -------------
 * Logging             | Output a log file.                  | bool   | false
 * Debug Context       | Create an OpenGL Debug Context.     | bool   | false
 * Show Frame Times    | Display update and render times.    | bool   | false
 * Screen Width        | Width of the game window.           | long   | 800
 * Screen Height       | Height of the game window.          | long   | 600
 * Fullscreen          | Play in fullscreen mode.            | bool   | false
 * Borderless          | Use a borderless window.            | bool   | false
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
