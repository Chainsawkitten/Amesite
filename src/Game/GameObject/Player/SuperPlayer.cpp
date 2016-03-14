#include "SuperPlayer.hpp"

using namespace GameObject;

SuperPlayer::SuperPlayer(Scene* scene) : SuperGameObject(scene){
    mActive = true;
    initalRespawnTime = respawnTimeLeft = 5.f;
    mState = LIGHTDAMAGE;
}

SuperPlayer::~SuperPlayer() {
}

bool SuperPlayer::Active() {
    return mActive;
}