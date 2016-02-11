#include "MainCamera.hpp"

MainCamera& MainCamera::GetInstance() {
    static MainCamera instance;

    return instance;
}

MainCamera::MainCamera() {

}

MainCamera::~MainCamera() {

}

MainCamera& MainCameraInstance() {
    return MainCamera::GetInstance();
}

void MainCamera::setMainCamera(Entity* camera) {
    mCamera = camera;
}

Entity& MainCamera::getMainCamera() {
    return *mCamera;
}