#pragma once

class Entity;

/// Singleton containing the main camera.
class MainCamera {
    public:
        /// Gets main camera instance.
        /**
         * @return The MainCamera instance.
         */
        static MainCamera& GetInstance();

        /// Sets the main camera entity.
        /**
         * @param camera Pointer to the camera that is the main camera for the scene.
         */
        void SetMainCamera(Entity* camera);

        /// Gets the main camera entity.
        /**
         * @return The main camera entity.
         */
        Entity& GetMainCamera();

    private:
        Entity* mCamera;
};

/// Gets the main camera.
/**
 * @return The %MainCamera instance
 */
MainCamera& MainCameraInstance();
