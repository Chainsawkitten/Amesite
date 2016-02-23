#pragma once

class Scene;

/// Handles the active scene and scene switching.
class Game {
    public:
        /// Create new game.
        Game();
        
        /// Set the active scene.
        /**
         * @param scene %Scene to set as active scene.
         */
        void SetScene(Scene* scene);
        
        /// Update the active scene.
        /**
         * @param deltaTime Time since last frame (in seconds).
         */
        void Update(float deltaTime);
        
        /// Free the active scene.
        void Free();
    
    private:
        Scene* mActiveScene;
};
