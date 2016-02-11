#include "AlbinScene.hpp"

#include <Resources.hpp>
#include <Audio/SoundSource.hpp>
#include <Audio/SoundBuffer.hpp>
#include <Audio/Listener.hpp>
#include <Util/Input.hpp>
#include <Util/Log.hpp>
#include "../Util/GameSettings.hpp"

AlbinScene::AlbinScene() {
    mSoundSystem.GetListener()->SetGain(GameSettings::GetInstance().GetDouble("Audio Volume"));
    
    mTestSoundBuffer = Resources().CreateSound("Resources/Laser.ogg");
    mTestSoundSource1 = new Audio::Sound(mTestSoundBuffer);
    mTestSoundSource2 = new Audio::Sound(mTestSoundBuffer);
    
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::SHOOT, InputHandler::MOUSE, GLFW_MOUSE_BUTTON_LEFT);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::LEFT, InputHandler::MOUSE, GLFW_MOUSE_BUTTON_RIGHT);
}

AlbinScene::~AlbinScene() {
    delete mTestSoundSource1;
    delete mTestSoundSource2;
    Resources().FreeSound(mTestSoundBuffer);
}

void AlbinScene::Update(float deltaTime) {
    if (Input()->Triggered(InputHandler::PLAYER_ONE, InputHandler::SHOOT))
        mTestSoundSource1->Play();
    
    if (Input()->Triggered(InputHandler::PLAYER_ONE, InputHandler::LEFT))
        mTestSoundSource2->Play();
}
