#include "Player1.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include "../../Component/Controller.hpp"
#include "../../Component/Health.hpp"
#include "../../Component/Spawner.hpp"
#include <Component/Transform.hpp>
#include <Component/Mesh.hpp>
#include <Component/Material.hpp>
#include <Component/Physics.hpp>
#include <Component/Collider2DCircle.hpp>
#include <Component/SpotLight.hpp>
#include <Component/PointLight.hpp>
#include <Component/Animation.hpp>
#include <Component/ParticleEmitter.hpp>
#include <Component/SoundSource.hpp>

#include "../../Util/ControlSchemes.hpp"

using namespace GameObject;

Player1::Player1(Scene* scene) : SuperPlayer(scene) {
    mActive = true;
    mRespawnTimer = 5;

    mState = LIGHTDAMAGE;

    mHealthyTexture = Resources().CreateTexture2DFromFile("Resources/player1_body_diff_healthy.png");
    mMediumDamageTexture = Resources().CreateTexture2DFromFile("Resources/player1_body_diff_medium_damage.png");
    mHeavyDamageTexture = Resources().CreateTexture2DFromFile("Resources/player1_body_diff_heavy_damage.png");

    mNode = CreateEntity();
    mNode->AddComponent<Component::Transform>()->scale *= 0.2f;
    mNode->AddComponent<Component::Controller>()->speed = 5000.f;
    mNode->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Move);
    mNode->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Shield);
    mNode->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Aim);
    mNode->GetComponent<Component::Controller>()->playerID = InputHandler::PLAYER_ONE;
    mNode->AddComponent<Component::Physics>()->velocityDragFactor = 3.f;
    mNode->AddComponent<Component::Health>()->removeOnLowHealth = false;
    mNode->GetComponent<Component::Health>()->health = 30.f;
    mNode->GetComponent<Component::Health>()->maxHealth = 30.f;
    mNode->GetComponent<Component::Health>()->faction = 0;
    mNode->AddComponent<Component::Collider2DCircle>()->radius = 10.f;
    mNode->AddComponent<Component::Animation>();
    Component::Animation::AnimationClip* idleNode = mNode->GetComponent<Component::Animation>()->CreateAnimationClip("idle");
    idleNode->CreateKeyFrame(glm::vec3(0.1f, 0.f, 0.f), 0.f, 0.f, 0, 1.5f, false, true);
    idleNode->CreateKeyFrame(glm::vec3(0.f, 0.1f, 0.1f), 0.f, 0.f, 0.f, 1.5f, false, true);
    idleNode->CreateKeyFrame(glm::vec3(0.1f, 0.1f, -0.1f), 0.f, 0.f, 0.f, 1.5f, false, true);
    idleNode->CreateKeyFrame(glm::vec3(-0.1f, 0.1f, 0.1f), 0.f, 0.f, 0.f, 1.5f, false, true);
    mNode->GetComponent<Component::Animation>()->Start("idle");

    mBody = CreateEntity();
    mBody->AddComponent<Component::RelativeTransform>()->parentEntity = mNode;
    mBody->AddComponent<Component::Mesh>()->geometry = mBodyModel = Resources().CreateOBJModel("Resources/player1_body.obj");
    mBody->AddComponent<Component::Material>();

    Resources().FreeTexture2D(mBody->GetComponent<Component::Material>()->diffuse);
    mBody->GetComponent<Component::Material>()->diffuse = mHealthyTexture;
    mBody->GetComponent<Component::Material>()->SetSpecular("Resources/player1_body_spec.png");
    mBody->GetComponent<Component::Material>()->SetGlow("Resources/player1_body_glow.png");
    mBody->AddComponent<Component::Animation>();

    Component::ParticleEmitter* emitter = mBody->AddComponent<Component::ParticleEmitter>();
    emitter->emitterType = Component::ParticleEmitter::POINT;
    emitter->maxEmitTime = 0.02;
    emitter->minEmitTime = 0.016;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 5.0;
    emitter->enabled = false;
    emitter->particleType.textureIndex = Component::ParticleEmitter::DUST;
    emitter->particleType.minLifetime = 1.f;
    emitter->particleType.maxLifetime = 2.f;
    emitter->particleType.minVelocity = glm::vec3(-.6f, 1.f, -.6f);
    emitter->particleType.maxVelocity = glm::vec3(.6f, 2.f, .6f);
    emitter->particleType.minSize = glm::vec2(.5f, .5f) * 2.f;
    emitter->particleType.maxSize = glm::vec2(.7f, .7f) * 2.f;
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(.2f, .8f, .2f);
    emitter->particleType.startAlpha = 1.f;
    emitter->particleType.midAlpha = 1.f;
    emitter->particleType.endAlpha = 0.f;

    mLight = CreateEntity();
    mLight->AddComponent<Component::RelativeTransform>()->Move(0, 1, 0);
    mLight->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mLight->AddComponent<Component::Animation>();
    mLight->AddComponent<Component::SpotLight>()->coneAngle = 45.f;
    mLight->GetComponent<Component::SpotLight>()->attenuation = 0.05f;

    mLeftTurret = CreateEntity();
    mLeftTurret->AddComponent<Component::RelativeTransform>()->Move(2.5f, 0, 13);
    mLeftTurret->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mLeftTurret->AddComponent<Component::Animation>();
    mLeftTurret->AddComponent<Component::Spawner>()->delay = 0.25f;
    mLeftTurret->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AimedFire);
    mLeftTurret->GetComponent<Component::Controller>()->playerID = InputHandler::PLAYER_ONE;
    Component::SoundSource* sound = mLeftTurret->AddComponent<Component::SoundSource>();
    mShootSound = Resources().CreateSound("Resources/Laser.ogg");
    sound->soundBuffer = mShootSound;
    sound->gain = 2.f;

    mRightTurret = CreateEntity();
    mRightTurret->AddComponent<Component::RelativeTransform>()->Move(-2.5f, 0, 13);
    mRightTurret->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mRightTurret->AddComponent<Component::Animation>();
    mRightTurret->AddComponent<Component::Spawner>()->delay = 0.25f;
    mRightTurret->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AimedFire);
    mRightTurret->GetComponent<Component::Controller>()->playerID = InputHandler::PLAYER_ONE;

    mFrontEngineLeft = CreateEntity();
    mFrontEngineLeft->AddComponent<Component::RelativeTransform>()->Move(8.5f, 0.f, 8.3f);
    mFrontEngineLeft->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mFrontEngineLeft->GetComponent<Component::RelativeTransform>()->scale *= 1.1f;
    mFrontEngineLeft->AddComponent<Component::Animation>();
    mFrontEngineLeft->AddComponent<Component::Mesh>()->geometry = mFrontEngineModel = Resources().CreateOBJModel("Resources/player1_frontEngine.obj");
    mFrontEngineLeft->AddComponent<Component::Material>();
    mFrontEngineLeft->GetComponent<Component::Material>()->SetDiffuse("Resources/player1_engine_diff.png");
    Entity* frontEngineLeftParticles = CreateEntity();
    frontEngineLeftParticles->AddComponent<Component::RelativeTransform>()->parentEntity = mFrontEngineLeft;
    frontEngineLeftParticles->GetComponent<Component::RelativeTransform>()->Move(0.f, -1.f, 0.f);
    AddEnginePartilces(frontEngineLeftParticles);

    mFrontEngineRight = CreateEntity();
    mFrontEngineRight->AddComponent<Component::RelativeTransform>()->Move(-8.5f, 0.f, 8.3f);
    mFrontEngineRight->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mFrontEngineRight->GetComponent<Component::RelativeTransform>()->scale *= 1.1f;
    mFrontEngineRight->GetComponent<Component::RelativeTransform>()->yaw = 180.f;
    mFrontEngineRight->AddComponent<Component::Animation>();
    mFrontEngineRight->AddComponent<Component::Mesh>()->geometry = mFrontEngineModel;
    mFrontEngineRight->AddComponent<Component::Material>();
    mFrontEngineRight->GetComponent<Component::Material>()->SetDiffuse("Resources/player1_engine_diff.png");
    Entity* frontEngineRightParticles = CreateEntity();
    frontEngineRightParticles->AddComponent<Component::RelativeTransform>()->parentEntity = mFrontEngineRight;
    frontEngineRightParticles->GetComponent<Component::RelativeTransform>()->Move(0.f, -1.f, 0.f);
    AddEnginePartilces(frontEngineRightParticles);

    mBackEngineLeft = CreateEntity();
    mBackEngineLeft->AddComponent<Component::RelativeTransform>()->Move(10.5f, 0.f, 0.f);
    mBackEngineLeft->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mBackEngineLeft->GetComponent<Component::RelativeTransform>()->scale *= 1.2f;
    mBackEngineLeft->AddComponent<Component::Animation>();
    mBackEngineLeft->AddComponent<Component::Mesh>()->geometry = mBackEngineModel = Resources().CreateOBJModel("Resources/player1_backEngine.obj");
    mBackEngineLeft->AddComponent<Component::Material>();
    mBackEngineLeft->GetComponent<Component::Material>()->SetDiffuse("Resources/player1_engine_diff.png");
    Entity* backEngineLeftParticles = CreateEntity();
    backEngineLeftParticles->AddComponent<Component::RelativeTransform>()->parentEntity = mBackEngineLeft;
    backEngineLeftParticles->GetComponent<Component::RelativeTransform>()->Move(0.f, -1.f, 0.f);
    AddEnginePartilces(backEngineLeftParticles);

    mBackEngineRight = CreateEntity();
    mBackEngineRight->AddComponent<Component::RelativeTransform>()->Move(-10.5f, 0.f, 0.f);
    mBackEngineRight->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mBackEngineRight->GetComponent<Component::RelativeTransform>()->scale *= 1.2f;
    mBackEngineRight->GetComponent<Component::RelativeTransform>()->yaw = 180.f;
    mBackEngineRight->AddComponent<Component::Animation>();
    mBackEngineRight->AddComponent<Component::Mesh>()->geometry = mBackEngineModel;
    mBackEngineRight->AddComponent<Component::Material>();
    mBackEngineRight->GetComponent<Component::Material>()->SetDiffuse("Resources/player1_engine_diff.png");
    Entity* backEngineRightParticles = CreateEntity();
    backEngineRightParticles->AddComponent<Component::RelativeTransform>()->parentEntity = mBackEngineRight;
    backEngineRightParticles->GetComponent<Component::RelativeTransform>()->Move(0.f, -1.f, 0.f);
    AddEnginePartilces(backEngineRightParticles);
}

Player1::~Player1() {
    if (mState != LIGHTDAMAGE)
        Resources().FreeTexture2D(mHealthyTexture);
    if (mState != MEDIUMDAMAGE)
        Resources().FreeTexture2D(mMediumDamageTexture);
    if (mState != HEAVYDAMAGE)
        Resources().FreeTexture2D(mHeavyDamageTexture);
    Resources().FreeSound(mShootSound);

    Resources().FreeOBJModel(mBodyModel);
    Resources().FreeOBJModel(mFrontEngineModel);
    Resources().FreeOBJModel(mBackEngineModel);
}

glm::vec3 Player1::GetPosition() {
    return mNode->GetComponent<Component::Transform>()->GetWorldPosition();
}

void Player1::SetPosition(glm::vec3 position) {
    mNode->GetComponent<Component::Transform>()->position = position;
}

Entity* Player1::GetNodeEntity() {
    return mNode;
}


float Player1::GetHealth() {
    return mNode->GetComponent<Component::Health>()->health;
}

void Player1::Activate() {

    mActive = true;
    mNode->GetComponent<Component::Controller>()->enabled = true;
    mLeftTurret->GetComponent<Component::Controller>()->enabled = true;
    mRightTurret->GetComponent<Component::Controller>()->enabled = true;
    mNode->GetComponent<Component::Health>()->health = mNode->GetComponent<Component::Health>()->maxHealth;

}

void Player1::Deactivate() {

    mActive = false;
    mNode->GetComponent<Component::Controller>()->enabled = false;
    mLeftTurret->GetComponent<Component::Controller>()->enabled = false;
    mRightTurret->GetComponent<Component::Controller>()->enabled = false;

}

void Player1::Update() {
    if (GetHealth() >= 2.f*(mNode->GetComponent<Component::Health>()->maxHealth / 3.f)) {
        mState = LIGHTDAMAGE;
        mBody->GetComponent<Component::Material>()->diffuse = mHealthyTexture;
    } else if (GetHealth() >= 1.f*(mNode->GetComponent<Component::Health>()->maxHealth / 3.f)) {
        mState = MEDIUMDAMAGE;
        mBody->GetComponent<Component::Material>()->diffuse = mMediumDamageTexture;
    } else {
        mState = HEAVYDAMAGE;
        mBody->GetComponent<Component::Material>()->diffuse = mHeavyDamageTexture;
    }
}

void Player1::AddEnginePartilces(Entity* entity) {
    Component::ParticleEmitter* emitter = entity->AddComponent<Component::ParticleEmitter>();
    emitter->emitterType = Component::ParticleEmitter::POINT;
    emitter->maxEmitTime = 0.02;
    emitter->minEmitTime = 0.016;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 0.0;
    emitter->particleType.textureIndex = Component::ParticleEmitter::BLUE;
    emitter->particleType.minLifetime = .01f * 2.f;
    emitter->particleType.maxLifetime = .02f * 2.f;
    emitter->particleType.minVelocity = glm::vec3(-.3f, 0.f, -.2f);
    emitter->particleType.maxVelocity = glm::vec3(.3f, 0.f, .2f);
    emitter->particleType.minSize = glm::vec2(.5f, .5f) * 2.f;
    emitter->particleType.maxSize = glm::vec2(.7f, .7f) * 2.f;
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(.8f, .8f, .8f);
    emitter->particleType.startAlpha = 1.f;
    emitter->particleType.midAlpha = 1.f;
    emitter->particleType.endAlpha = 0.f;
}
