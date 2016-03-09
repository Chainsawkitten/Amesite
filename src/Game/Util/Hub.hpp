#pragma once

namespace GameObject {
    class Camera;
    class Player1;
    class Player2;
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

        /// Sets player1.
        /**
         * @param player1 Pointer to the player1 that is the hub for the scene.
         */
        void SetPlayer1(GameObject::Player1* player1);

        /// Gets the player1.
        /**
         * @return The player1.
         */
        GameObject::Player1* GetPlayer1();

        /// Sets player2.
        /**
         * @param player2 Pointer to the player1 that is the hub for the scene.
         */
        void SetPlayer2(GameObject::Player2* player2);

        /// Gets the player2.
        /**
         * @return The player2.
         */
        GameObject::Player2* GetPlayer2();

    private:
        GameObject::Camera* mCamera;
        GameObject::Player1* mPlayer1;
        GameObject::Player2* mPlayer2;
};

/// Gets the hub.
/**
 * @return The %Hub instance
 */
Hub& HubInstance();
