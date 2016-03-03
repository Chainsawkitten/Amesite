#pragma once

class Scene;

/// Handles the active scene and scene switching.
class Game {
    public:
        /// Get the instance of the class.
        /**
         * @return The %Game instance.
         */
        static Game& GetInstance();
        
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
        Scene* mNextScene;
        
        Game();
        
        Game(Game const&);
        void operator=(Game const&);
};
