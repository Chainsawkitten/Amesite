#pragma once

#include <Util/Settings.hpp>

/// %Settings for the game.
/**
 * Available settings:
 * Name                | Description                         | Type   | Default Value
 * ------------------- | ----------------------------------- | ------ | -------------
 * Logging             | Output a log file.                  | bool   | false
 * Debug Context       | Create an OpenGL Debug Context.     | bool   | false
 * Show Notifications  | Show notification messages.         | bool   | false
 * Show Frame Times    | Display update and render times.    | bool   | false
 * Screen Width        | Width of the game window.           | long   | 800
 * Screen Height       | Height of the game window.          | long   | 600
 * Fullscreen          | Play in fullscreen mode.            | bool   | false
 * Borderless          | Use a borderless window.            | bool   | false
 * Target FPS          | FPS to aim for when rendering.      | long   | 60
 * VSync               | Whether to enable vsync.            | bool   | false
 * FXAA                | Whether to enable fxaa.             | bool   | false
 * Gamma               | Gamma brightness.                   | double | 2.2
 * Audio Volume        | How loud to play sounds.            | double | 1.0
 * Aim Deadzone        | Deadzone on controller aim vector.  | double | 0.3
 * Move Deadzone       | Deadzone on controller move vector. | double | 0.3
 * Show Splash Screen  | Show logo when starting game.       | bool   | true
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
