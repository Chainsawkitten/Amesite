#pragma once

#include <Engine/Scene/Scene.hpp>
#include <vector>

class Texture2D;
class Font;

/// Splash screen shown when winning the game.
/**
* Shows statistics and then continues to main scene.
*/
class WinScene : public Scene {
    public:
        /// Create win scene.
        /**
        * @param totalTime The total time played.
        * @param enemiesKilled The amount of enemies killed.
        * @param timesDied The amount of times the players died.
        */
        WinScene(float totalTime, int enemiesKilled, int timesDied);

        /// Destructor
        ~WinScene();

        /// Update the scene.
        /**
        *@param deltaTime Time since last frame.
        */
        void Update(float deltaTime);

    private:
        Texture2D* mPortalTexture;
        Font* mFont;
        std::string mWinMessage;
        float mElapsedTime;
};
