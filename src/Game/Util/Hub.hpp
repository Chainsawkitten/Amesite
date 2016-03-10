#pragma once
#include <vector>

namespace GameObject {
    class Camera;
    class SuperPlayer;
}

/// Singleton containing the main camera and players.
class Hub {
    public:
        /// Gets hub instance.
        /**
         * @return The MainCamera instance.
         */
        static Hub& GetInstance();

        /// Sets the main camera.
        /**
         * @param camera Pointer to the camera that is the main camera for the scene.
         */
        void SetMainCamera(GameObject::Camera* camera);

        /// Gets the main camera.
        /**
         * @return The main camera entity.
         */
        GameObject::Camera& GetMainCamera();

        ///Vector containing all the players.
        std::vector<GameObject::SuperPlayer*> mPlayers;
    private:
        GameObject::Camera* mCamera;
};

/// Gets the hub.
/**
 * @return The %Hub instance
 */
Hub& HubInstance();
