#include "SuperPlayer.hpp"

using namespace GameObject;

SuperPlayer::SuperPlayer(Scene* scene) : SuperGameObject(scene){
    mActive = true;
    mRespawnTimer = 5.f;
    mState = LIGHTDAMAGE;
}

SuperPlayer::~SuperPlayer() {
}

bool SuperPlayer::Active() {
    return mActive;
}
