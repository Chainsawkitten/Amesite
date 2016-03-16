#include "Hub.hpp"

#include "../GameObject/Camera.hpp"
#include "../GameObject/Player/Player1.hpp"
#include "../GameObject/Player/Player2.hpp"

#include <util/Input.hpp>
#include "../Util/GameSettings.hpp"

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
    SetPlayer1Joystick(GameSettings::GetInstance().GetBool("Player One Joystick Aim"));
}

void Hub::SetPlayer1Joystick(bool joystick) {
    GameObject::Player1* player1 = static_cast<GameObject::Player1*>(mPlayers[0]);
    // Joystick, one player
    if (joystick && mPlayers.size() == 1) {
        player1->SetJoystickAim(joystick);
        player1->SetPlayerID(InputHandler::PLAYER_ONE);
    }
    // Mouse, one player
    else if (!joystick && mPlayers.size() == 1) {
        player1->SetJoystickAim(joystick);
        player1->SetPlayerID(InputHandler::PLAYER_TWO);
    }
    // Joystick, two players
    else if (joystick && mPlayers.size() == 2) {
        player1->SetJoystickAim(joystick);
        player1->SetPlayerID(InputHandler::PLAYER_TWO);
    }
    // Mouse, two players
    else {
        player1->SetJoystickAim(joystick);
        player1->SetPlayerID(InputHandler::PLAYER_TWO);
    }
}

Hub::Hub() {
    mDisabledPlayer = nullptr;
}

Hub::~Hub() {

}
