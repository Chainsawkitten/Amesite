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

void Hub::SetPlayer2State(bool enable) {
    if (enable && mDisabledPlayer != nullptr) {
        glm::vec3 position = mDisabledPlayer->GetPosition();
        mDisabledPlayer->SetPosition(glm::vec3(position.x, 0.f, position.z));
        mPlayers.push_back(mDisabledPlayer);
        mDisabledPlayer = nullptr;
    } else if (!enable && mDisabledPlayer == nullptr) {
        mDisabledPlayer = mPlayers[1];
        glm::vec3 position = mDisabledPlayer->GetPosition();
        mDisabledPlayer->SetPosition(glm::vec3(position.x,-30.f, position.z));
        mPlayers.erase(mPlayers.begin() + 1);
    }
}

Hub::Hub() {
    mDisabledPlayer = nullptr;
}

Hub::~Hub() {

}
