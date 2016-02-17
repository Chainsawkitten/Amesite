#include "AlbinScene.hpp"

#include <Resources.hpp>
#include <Audio/SoundBuffer.hpp>
#include <Audio/Listener.hpp>

#include "../Util/GameSettings.hpp"

#include <Entity/Entity.hpp>
#include <Component/Transform.hpp>
#include <Component/Physics.hpp>
#include <Component/SoundSource.hpp>
#include <Component/Listener.hpp>

#include "../Util/GameEntityFactory.hpp"
#include "../GameObject/Camera.hpp"
#include "../GameObject/Player.hpp"
#include "../Util/MainCamera.hpp"
#include <Util/Input.hpp>

#include <MainWindow.hpp>
#include <PostProcessing/PostProcessing.hpp>
#include <PostProcessing/FXAAFilter.hpp>
#include <PostProcessing/GammaCorrectionFilter.hpp>

using namespace GameObject;

AlbinScene::AlbinScene() {
    mSoundSystem.GetListener()->SetGain(GameSettings::GetInstance().GetDouble("Audio Volume"));
    
    // Assign input
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::MOVE_X, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::MOVE_Z, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::AIM_X, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::AIM_Z, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::SHOOT, InputHandler::JOYSTICK, InputHandler::RIGHT_BUMPER);
    
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::MOVE_X, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::MOVE_Z, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::AIM_X, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::AIM_Z, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::SHOOT, InputHandler::JOYSTICK, InputHandler::RIGHT_BUMPER);
    
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::UP, InputHandler::KEYBOARD, GLFW_KEY_W);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::DOWN, InputHandler::KEYBOARD, GLFW_KEY_S);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::RIGHT, InputHandler::KEYBOARD, GLFW_KEY_D);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::LEFT, InputHandler::KEYBOARD, GLFW_KEY_A);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::SHOOT, InputHandler::MOUSE, GLFW_MOUSE_BUTTON_1);
    
    mTestSoundBuffer = Resources().CreateSound("Resources/Laser.ogg");
    
    s1 = CreateEntity();
    Component::Transform* transform = s1->AddComponent<Component::Transform>();
    transform->position = glm::vec3(-10.f, 0.f, 0.f);
    s1->AddComponent<Component::Physics>()->velocity = glm::vec3(1.f, 0.f, 0.f);
    Component::SoundSource* ss1 = s1->AddComponent<Component::SoundSource>();
    ss1->soundBuffer = mTestSoundBuffer;
    ss1->loop = AL_TRUE;
    ss1->Play();
    
    // Bind scene to gameEntityCreator
    GameEntityCreator().SetScene(this);
    
    // Create main camera
    Camera* mainCamera = GameEntityCreator().CreateCamera(glm::vec3(0.f, 40.f, 0.f), glm::vec3(0.f, 90.f, 0.f));
    mMainCamera = mainCamera->body;
    mMainCamera->AddComponent<Component::Listener>();
    MainCameraInstance().SetMainCamera(mMainCamera);
    
    // Create players
    Player* player1 = GameEntityCreator().CreatePlayer(glm::vec3(-4.f, 0.f, 0.f), InputHandler::PLAYER_ONE);
    Player* player2 = GameEntityCreator().CreatePlayer(glm::vec3(1.f, 0.f, 1.f), InputHandler::PLAYER_TWO);
    
    mPlayers.push_back(player1->body);
    mPlayers.push_back(player2->body);
    
    postProcessing = new PostProcessing(MainWindow::GetInstance()->GetSize());
    fxaaFilter = new FXAAFilter();
    gammaCorrectionFilter = new GammaCorrectionFilter();
}

AlbinScene::~AlbinScene() {
    RemoveEntity(s1);
    
    Resources().FreeSound(mTestSoundBuffer);
    
    delete fxaaFilter;
    delete gammaCorrectionFilter;
    delete postProcessing;
}

void AlbinScene::Update(float deltaTime) {
    // Update controllable objects.
    mControllerSystem.Update(*this, deltaTime);
    
    // Update physics.
    mPhysicsSystem.Update(*this, deltaTime);
    
    // Update camera.
    //UpdateCamera(mMainCamera, mPlayers);
    
    // Updates model matrices for this frame.
    UpdateModelMatrices();
    
    // Update sounds.
    mSoundSystem.Update(*this);
    
    // Render.
    mRenderSystem.Render(*this, postProcessing->GetRenderTarget());
    
    // Apply post-processing effects.
    if (GameSettings::GetInstance().GetBool("FXAA")) {
        fxaaFilter->SetScreenSize(MainWindow::GetInstance()->GetSize());
        postProcessing->ApplyFilter(fxaaFilter);
    }
    
    gammaCorrectionFilter->SetBrightness(GameSettings::GetInstance().GetDouble("Gamma"));
    postProcessing->ApplyFilter(gammaCorrectionFilter);
    
    postProcessing->Render();
}
