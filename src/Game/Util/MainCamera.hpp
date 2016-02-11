#pragma once

class Entity;

class MainCamera {
    public:
        ///Constructor
        MainCamera();

        ///Destructor
        ~MainCamera();

        ///Gets main camera instance.
        /**
        * @return The MainCamera instance.
        */
        static MainCamera& MainCamera::GetInstance();

        ///Sets the main camera entity.
        /**
        * @param camera Pointer to the camera that is the main camera for the scene.
        */
        void setMainCamera(Entity* camera);

        ///Gets the main camera entity.
        /**
        * @return The main camera entity.
        */
        Entity& getMainCamera();

    private:
        ///Main camera entity.
        Entity* mCamera;
};

/// Gets the main camera.
/**
* @return The %MainCamera instance
*/
MainCamera& MainCameraInstance();