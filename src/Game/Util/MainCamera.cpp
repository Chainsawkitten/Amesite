#include "MainCamera.hpp"

MainCamera& MainCamera::GetInstance() {
    static MainCamera instance;

    return instance;
}

MainCamera& MainCameraInstance() {
    return MainCamera::GetInstance();
}

void MainCamera::SetMainCamera(Entity* camera) {
    mCamera = camera;
}

Entity& MainCamera::GetMainCamera() {
    return *mCamera;
}