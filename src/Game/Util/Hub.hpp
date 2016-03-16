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


        /// Set state of player 2
        /**
         * @param enable Set true if two players.
         */
        void SetPlayer2State(bool enable);

        ///Vector containing all the players.
        std::vector<GameObject::SuperPlayer*> mPlayers;
    private:
        Hub();
        ~Hub();
        GameObject::Camera* mCamera;
        GameObject::SuperPlayer* mDisabledPlayer;
};

/// Gets the hub.
/**
 * @return The %Hub instance
 */
Hub& HubInstance();
