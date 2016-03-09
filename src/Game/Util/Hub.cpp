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

void Hub::SetPlayer1(Player1* player1) {
    mPlayer1 = player1;
}

Player1* Hub::GetPlayer1() {
    return mPlayer1;
}

void Hub::SetPlayer2(Player2* player2) {
    mPlayer2 = player2;
}

Player2* Hub::GetPlayer2() {
    return mPlayer2;
}

Hub& HubInstance() {
    return Hub::GetInstance();
}
