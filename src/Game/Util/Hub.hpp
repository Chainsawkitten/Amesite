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

        /// Set control state of player 1
        /**
         * @param joystick Player one joystick controlled or not
         */
        void SetPlayer1Joystick(bool joystick);

        ///Vector containing all the players.
        std::vector<GameObject::SuperPlayer*> mPlayers;

        ///Clears the hub.
        void Clear();
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
