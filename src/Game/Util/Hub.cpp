#include "Hub.hpp"

#include "../GameObject/Camera.hpp"
#include "../GameObject/Player/Player1.hpp"
#include "../GameObject/Player/Player2.hpp"

using namespace GameObject;

Hub& Hub::GetInstance() {
    static Hub instance;

    return instance;
}

Hub& HubInstace() {
    return Hub::GetInstance();
}

void Hub::SetMainCamera(Camera* camera) {
    mCamera = camera;
}

Camera& Hub::GetMainCamera() {
    return *mCamera;
}

Hub& HubInstance() {
    return Hub::GetInstance();
}
