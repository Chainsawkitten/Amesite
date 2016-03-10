#include "Player1.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include "../../Component/Controller.hpp"
#include "../../Component/Health.hpp"
#include "../../Component/Spawner.hpp"
#include "../../Component/Update.hpp"
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
    Component::ParticleEmitter* emitter = mNode->AddComponent<Component::ParticleEmitter>();
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

    mNode->AddComponent<Component::Update>()->updateFunction = std::bind(&Player1::mUpdateFunction, this);

    mBody = CreateEntity();
    mBody->AddComponent<Component::RelativeTransform>()->parentEntity = mNode;
    mBody->AddComponent<Component::Mesh>()->geometry = mBodyModel = Resources().CreateOBJModel("Resources/player1_body.obj");
    mBody->AddComponent<Component::Material>();

    Resources().FreeTexture2D(mBody->GetComponent<Component::Material>()->diffuse);
    mBody->GetComponent<Component::Material>()->diffuse = mHealthyTexture;
    mBody->GetComponent<Component::Material>()->SetSpecular("Resources/player1_body_spec.png");
    mBody->GetComponent<Component::Material>()->SetGlow("Resources/player1_body_glow.png");
    mBody->AddComponent<Component::Animation>();

    mLight = CreateEntity();
    mLight->AddComponent<Component::RelativeTransform>()->Move(0, 1, 0);
    mLight->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mLight->GetComponent<Component::RelativeTransform>()->pitch = 15.f;
    mLight->AddComponent<Component::Animation>();
    mLight->AddComponent<Component::SpotLight>()->coneAngle = 20.f;
    mLight->GetComponent<Component::SpotLight>()->attenuation = 0.1f;

    mBottomLight = CreateEntity();
    mBottomLight->AddComponent<Component::RelativeTransform>()->Move(0.f, -7.f, 0.f);
    mBottomLight->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mBottomLight->AddComponent<Component::Animation>();
    mBottomLight->AddComponent<Component::PointLight>();
    mBottomLight->GetComponent<Component::PointLight>()->color = glm::vec3(1.f, 1.f, 1.f);
    mBottomLight->GetComponent<Component::PointLight>()->attenuation = 0.8f;
    

    mTurretBodyModel = Resources().CreateOBJModel("Resources/player1_turret_body.obj");
    mTurretBarrelModel = Resources().CreateOBJModel("Resources/player1_turret_barrel.obj");

    // Left Turret
    mLeftTurretBody = CreateEntity();
    mLeftTurretBody->AddComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mLeftTurretBody->GetComponent<Component::RelativeTransform>()->Move(3.f, -3.f, 10.f);
    mLeftTurretBody->GetComponent<Component::RelativeTransform>()->roll = -45.f;
    mLeftTurretBody->GetComponent<Component::RelativeTransform>()->scale *= 0.6f;
    mLeftTurretBody->AddComponent<Component::Animation>();
    mLeftTurretBody->AddComponent<Component::Mesh>()->geometry = mTurretBodyModel;
    mLeftTurretBody->AddComponent<Component::Material>()->SetDiffuse("Resources/player1_engine_diff.png");

    mLeftTurretBarrel = CreateEntity();
    mLeftTurretBarrel->AddComponent<Component::RelativeTransform>()->parentEntity = mLeftTurretBody;
    mLeftTurretBarrel->AddComponent<Component::Animation>();
    mLeftTurretBarrel->AddComponent<Component::Mesh>()->geometry = mTurretBarrelModel;
    mLeftTurretBarrel->AddComponent<Component::Material>()->SetDiffuse("Resources/player1_engine_diff.png");

    mLeftSpawnNode = CreateEntity();
    mLeftSpawnNode->AddComponent<Component::RelativeTransform>()->parentEntity = mLeftTurretBarrel;
    mLeftSpawnNode->GetComponent<Component::RelativeTransform>()->Move(0.f, 0.f, 10.f);
    mLeftSpawnNode->AddComponent<Component::Animation>();
    mLeftSpawnNode->AddComponent<Component::Spawner>()->delay = 0.3f;
    mLeftSpawnNode->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AimedFire);
    mLeftSpawnNode->GetComponent<Component::Controller>()->playerID = InputHandler::PLAYER_ONE;
    Component::SoundSource* sound = mLeftSpawnNode->AddComponent<Component::SoundSource>();
    mShootSound = Resources().CreateSound("Resources/Laser.ogg");
    sound->soundBuffer = mShootSound;
    sound->gain = 2.f;

    // Right Turret
    mRightTurretBody = CreateEntity();
    mRightTurretBody->AddComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mRightTurretBody->GetComponent<Component::RelativeTransform>()->Move(-3.f, -3.f, 10.f);
    mRightTurretBody->GetComponent<Component::RelativeTransform>()->roll = 45.f;
    mRightTurretBody->GetComponent<Component::RelativeTransform>()->scale *= 0.6f;
    mRightTurretBody->AddComponent<Component::Animation>();
    mRightTurretBody->AddComponent<Component::Mesh>()->geometry = mTurretBodyModel;
    mRightTurretBody->AddComponent<Component::Material>()->SetDiffuse("Resources/player1_engine_diff.png");

    mRightTurretBarrel = CreateEntity();
    mRightTurretBarrel->AddComponent<Component::RelativeTransform>()->parentEntity = mRightTurretBody;
    mRightTurretBarrel->AddComponent<Component::Animation>();
    mRightTurretBarrel->AddComponent<Component::Mesh>()->geometry = mTurretBarrelModel;
    mRightTurretBarrel->AddComponent<Component::Material>()->SetDiffuse("Resources/player1_engine_diff.png");

    mRightSpawnNode = CreateEntity();
    mRightSpawnNode->AddComponent<Component::RelativeTransform>()->parentEntity = mRightTurretBarrel;
    mRightSpawnNode->GetComponent<Component::RelativeTransform>()->Move(0.f, 0.f, 10.f);
    mRightSpawnNode->AddComponent<Component::Animation>();
    mRightSpawnNode->AddComponent<Component::Spawner>()->delay = 0.3f;
    mRightSpawnNode->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AimedFire);
    mRightSpawnNode->GetComponent<Component::Controller>()->playerID = InputHandler::PLAYER_ONE;

    Entity* pointLight;
    // Engine
    mFrontEngineLeft = CreateEntity();
    mFrontEngineLeft->AddComponent<Component::RelativeTransform>()->Move(8.5f, 0.f, 8.3f);
    mFrontEngineLeft->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mFrontEngineLeft->GetComponent<Component::RelativeTransform>()->scale *= 1.1f;
    mFrontEngineLeft->AddComponent<Component::Animation>();
    mFrontEngineLeft->AddComponent<Component::Mesh>()->geometry = mFrontEngineModel = Resources().CreateOBJModel("Resources/player1_frontEngine.obj");
    mFrontEngineLeft->AddComponent<Component::Material>();
    mFrontEngineLeft->GetComponent<Component::Material>()->SetDiffuse("Resources/player1_engine_diff.png");
    mFrontEngineLeftParticles = CreateEntity();
    mFrontEngineLeftParticles->AddComponent<Component::RelativeTransform>()->parentEntity = mFrontEngineLeft;
    AddEnginePartilces(mFrontEngineLeftParticles);
    pointLight = CreateEntity();
    pointLight->AddComponent<Component::PointLight>();
    pointLight->GetComponent<Component::PointLight>()->attenuation = 5.f;
    pointLight->AddComponent<Component::RelativeTransform>()->parentEntity = mFrontEngineLeft;
    pointLight->GetComponent<Component::RelativeTransform>()->Move(0, 10, 0);
    emitter = mFrontEngineLeft->AddComponent<Component::ParticleEmitter>();
    emitter->emitterType = Component::ParticleEmitter::POINT;
    emitter->maxEmitTime = 0.02;
    emitter->minEmitTime = 0.016;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 5.0;
    emitter->enabled = false;
    emitter->particleType.textureIndex = Component::ParticleEmitter::DUST;
    emitter->particleType.minLifetime = 0.2f;
    emitter->particleType.maxLifetime = 0.5f;
    emitter->particleType.minVelocity = glm::vec3(-.3f, 2.f, -.3f);
    emitter->particleType.maxVelocity = glm::vec3(.3f, 3.f, .3f);
    emitter->particleType.minSize = glm::vec2(.5f, .5f);
    emitter->particleType.maxSize = glm::vec2(.7f, .7f);
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(.2f, .2f, .2f);
    emitter->particleType.startAlpha = 1.f;
    emitter->particleType.midAlpha = 1.f;
    emitter->particleType.endAlpha = 0.f;

    mFrontEngineRight = CreateEntity();
    mFrontEngineRight->AddComponent<Component::RelativeTransform>()->Move(-8.5f, 0.f, 8.3f);
    mFrontEngineRight->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mFrontEngineRight->GetComponent<Component::RelativeTransform>()->scale *= 1.1f;
    mFrontEngineRight->GetComponent<Component::RelativeTransform>()->yaw = 180.f;
    mFrontEngineRight->AddComponent<Component::Animation>();
    mFrontEngineRight->AddComponent<Component::Mesh>()->geometry = mFrontEngineModel;
    mFrontEngineRight->AddComponent<Component::Material>();
    mFrontEngineRight->GetComponent<Component::Material>()->SetDiffuse("Resources/player1_engine_diff.png");
    mFrontEngineRightParticles = CreateEntity();
    mFrontEngineRightParticles->AddComponent<Component::RelativeTransform>()->parentEntity = mFrontEngineRight;
    AddEnginePartilces(mFrontEngineRightParticles);
    pointLight = CreateEntity();
    pointLight->AddComponent<Component::PointLight>();
    pointLight->GetComponent<Component::PointLight>()->attenuation = 5.f;
    pointLight->AddComponent<Component::RelativeTransform>()->parentEntity = mFrontEngineRight;
    pointLight->GetComponent<Component::RelativeTransform>()->Move(0, 10, 0);
    emitter = mFrontEngineRight->AddComponent<Component::ParticleEmitter>();
    emitter->emitterType = Component::ParticleEmitter::POINT;
    emitter->maxEmitTime = 0.02;
    emitter->minEmitTime = 0.016;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 5.0;
    emitter->enabled = false;
    emitter->particleType.textureIndex = Component::ParticleEmitter::DUST;
    emitter->particleType.minLifetime = 0.2f;
    emitter->particleType.maxLifetime = 0.5f;
    emitter->particleType.minVelocity = glm::vec3(-.3f, 2.f, -.3f);
    emitter->particleType.maxVelocity = glm::vec3(.3f, 3.f, .3f);
    emitter->particleType.minSize = glm::vec2(.5f, .5f);
    emitter->particleType.maxSize = glm::vec2(.7f, .7f);
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(.2f, .2f, .2f);
    emitter->particleType.startAlpha = 1.f;
    emitter->particleType.midAlpha = 1.f;
    emitter->particleType.endAlpha = 0.f;

    mBackEngineLeft = CreateEntity();
    mBackEngineLeft->AddComponent<Component::RelativeTransform>()->Move(10.5f, 0.f, 0.f);
    mBackEngineLeft->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mBackEngineLeft->GetComponent<Component::RelativeTransform>()->scale *= 1.2f;
    mBackEngineLeft->AddComponent<Component::Animation>();
    mBackEngineLeft->AddComponent<Component::Mesh>()->geometry = mBackEngineModel = Resources().CreateOBJModel("Resources/player1_backEngine.obj");
    mBackEngineLeft->AddComponent<Component::Material>();
    mBackEngineLeft->GetComponent<Component::Material>()->SetDiffuse("Resources/player1_engine_diff.png");
    mBackEngineLeftParticles = CreateEntity();
    mBackEngineLeftParticles->AddComponent<Component::RelativeTransform>()->parentEntity = mBackEngineLeft;
    AddEnginePartilces(mBackEngineLeftParticles);
    //pointLight = CreateEntity();
    //pointLight->AddComponent<Component::PointLight>();
    //pointLight->GetComponent<Component::PointLight>()->attenuation = 5.f;
    //pointLight->AddComponent<Component::RelativeTransform>()->parentEntity = mFrontEngineLeft;
    //pointLight->GetComponent<Component::RelativeTransform>()->Move(0, 10, 0);
    emitter = mBackEngineLeft->AddComponent<Component::ParticleEmitter>();
    emitter->emitterType = Component::ParticleEmitter::POINT;
    emitter->maxEmitTime = 0.02;
    emitter->minEmitTime = 0.016;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 5.0;
    emitter->enabled = false;
    emitter->particleType.textureIndex = Component::ParticleEmitter::DUST;
    emitter->particleType.minLifetime = 0.2f;
    emitter->particleType.maxLifetime = 0.5f;
    emitter->particleType.minVelocity = glm::vec3(-.3f, 2.f, -.3f);
    emitter->particleType.maxVelocity = glm::vec3(.3f, 3.f, .3f);
    emitter->particleType.minSize = glm::vec2(.5f, .5f);
    emitter->particleType.maxSize = glm::vec2(.7f, .7f);
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(.2f, .2f, .2f);
    emitter->particleType.startAlpha = 1.f;
    emitter->particleType.midAlpha = 1.f;
    emitter->particleType.endAlpha = 0.f;

    mBackEngineRight = CreateEntity();
    mBackEngineRight->AddComponent<Component::RelativeTransform>()->Move(-10.5f, 0.f, 0.f);
    mBackEngineRight->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mBackEngineRight->GetComponent<Component::RelativeTransform>()->scale *= 1.2f;
    mBackEngineRight->GetComponent<Component::RelativeTransform>()->yaw = 180.f;
    mBackEngineRight->AddComponent<Component::Animation>();
    mBackEngineRight->AddComponent<Component::Mesh>()->geometry = mBackEngineModel;
    mBackEngineRight->AddComponent<Component::Material>();
    mBackEngineRight->GetComponent<Component::Material>()->SetDiffuse("Resources/player1_engine_diff.png");
    mBackEngineRightParticles = CreateEntity();
    mBackEngineRightParticles->AddComponent<Component::RelativeTransform>()->parentEntity = mBackEngineRight;
    AddEnginePartilces(mBackEngineRightParticles);
    //pointLight = CreateEntity();
    //pointLight->AddComponent<Component::PointLight>();
    //pointLight->GetComponent<Component::PointLight>()->attenuation = 5.f;
    //pointLight->AddComponent<Component::RelativeTransform>()->parentEntity = mBackEngineRight;
    //pointLight->GetComponent<Component::RelativeTransform>()->Move(0, 10, 0);
    emitter = mBackEngineRight->AddComponent<Component::ParticleEmitter>();
    emitter->emitterType = Component::ParticleEmitter::POINT;
    emitter->maxEmitTime = 0.02;
    emitter->minEmitTime = 0.016;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 5.0;
    emitter->enabled = false;
    emitter->particleType.textureIndex = Component::ParticleEmitter::DUST;
    emitter->particleType.minLifetime = 0.2f;
    emitter->particleType.maxLifetime = 0.5f;
    emitter->particleType.minVelocity = glm::vec3(-.3f, 2.f, -.3f);
    emitter->particleType.maxVelocity = glm::vec3(.3f, 3.f, .3f);
    emitter->particleType.minSize = glm::vec2(.5f, .5f);
    emitter->particleType.maxSize = glm::vec2(.7f, .7f);
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(.2f, .2f, .2f);
    emitter->particleType.startAlpha = 1.f;
    emitter->particleType.midAlpha = 1.f;
    emitter->particleType.endAlpha = 0.f;

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
    Resources().FreeOBJModel(mTurretBarrelModel);
    Resources().FreeOBJModel(mTurretBodyModel);
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
    mLeftSpawnNode->GetComponent<Component::Controller>()->enabled = true;
    mRightSpawnNode->GetComponent<Component::Controller>()->enabled = true;
    mNode->GetComponent<Component::Health>()->health = mNode->GetComponent<Component::Health>()->maxHealth;
    mNode->GetComponent<Component::ParticleEmitter>()->enabled = false;
}

void Player1::Deactivate() {

    mActive = false;
    mNode->GetComponent<Component::Controller>()->enabled = false;
    mLeftSpawnNode->GetComponent<Component::Controller>()->enabled = false;
    mRightSpawnNode->GetComponent<Component::Controller>()->enabled = false;
    mNode->GetComponent<Component::ParticleEmitter>()->enabled = true;
    mNode->GetComponent<Component::Physics>()->acceleration = glm::vec3(0, 0, 0);

}

void Player1::AddEnginePartilces(Entity* entity) {
    entity->GetComponent<Component::RelativeTransform>()->Move(0.f, -1.f, 0.f);
    Component::ParticleEmitter* emitter = entity->AddComponent<Component::ParticleEmitter>();
    emitter->emitterType = Component::ParticleEmitter::POINT;
    emitter->maxEmitTime = 0.02 / 10.f;
    emitter->minEmitTime = 0.016 / 10.f;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 0.0;
    emitter->particleType.textureIndex = Component::ParticleEmitter::BLUE;
    emitter->particleType.minLifetime = .04f;
    emitter->particleType.maxLifetime = .08f;
    emitter->particleType.minVelocity = glm::vec3(0.f, -10.f, 0.f);
    emitter->particleType.maxVelocity = glm::vec3(.3f, -15.f, .3f);
    emitter->particleType.minSize = glm::vec2(.5f, .5f) * 2.f;
    emitter->particleType.maxSize = glm::vec2(.7f, .7f) * 2.f;
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(.8f, .8f, .8f);
    emitter->particleType.startAlpha = 1.f;
    emitter->particleType.midAlpha = 1.f;
    emitter->particleType.endAlpha = 0.f;
}

void Player1::mUpdateFunction() {
    // Update health texture
    if (GetHealth() >= 2.f*(mNode->GetComponent<Component::Health>()->maxHealth / 3.f)) {
        mState = LIGHTDAMAGE;
        mLight->GetComponent<Component::SpotLight>()->color = glm::vec3(1.f, 1.f, 1.f);
        mBottomLight->GetComponent<Component::PointLight>()->color = glm::vec3(0.f, 1.f, 0.f);
        mBody->GetComponent<Component::Material>()->diffuse = mHealthyTexture;

        mFrontEngineLeft->GetComponent<Component::ParticleEmitter>()->enabled = false;
        mFrontEngineRight->GetComponent<Component::ParticleEmitter>()->enabled = false;
        mBackEngineLeft->GetComponent<Component::ParticleEmitter>()->enabled = false;
        mBackEngineRight->GetComponent<Component::ParticleEmitter>()->enabled = false;

    } else if (GetHealth() >= 1.f*(mNode->GetComponent<Component::Health>()->maxHealth / 3.f)) {
        mState = MEDIUMDAMAGE;
        mLight->GetComponent<Component::SpotLight>()->color = glm::vec3(1.f, 1.0f, 0.0f);
        mBottomLight->GetComponent<Component::PointLight>()->color = glm::vec3(1.f, 1.f, 0.f);
        mBody->GetComponent<Component::Material>()->diffuse = mMediumDamageTexture;
    } else {
        mState = HEAVYDAMAGE;
        mLight->GetComponent<Component::SpotLight>()->color = glm::vec3(1.f, 0.0f, 0.0f);
        mBottomLight->GetComponent<Component::PointLight>()->color = glm::vec3(1.f, 0.f, 0.f);
        mBody->GetComponent<Component::Material>()->diffuse = mHeavyDamageTexture;

        mFrontEngineLeft->GetComponent<Component::ParticleEmitter>()->enabled = true;
        mFrontEngineRight->GetComponent<Component::ParticleEmitter>()->enabled = true;
        mBackEngineLeft->GetComponent<Component::ParticleEmitter>()->enabled = true;
        mBackEngineRight->GetComponent<Component::ParticleEmitter>()->enabled = true;

    }

    glm::vec3 velocity = mNode->GetComponent<Component::Physics>()->velocity;
    glm::vec3 direction = mNode->GetComponent<Component::Transform>()->GetWorldDirection();

    // Update factors
    float velocityFactor = glm::length(mNode->GetComponent<Component::Physics>()->velocity) / glm::length(mNode->GetComponent<Component::Physics>()->maxVelocity);
    float pitchFactor = 0.f;
    float rollFactor = 0.f;
    if (glm::length(velocity) > 0.01f) {
        pitchFactor = glm::dot(glm::normalize(velocity), direction);
        rollFactor = glm::dot(glm::normalize(velocity), glm::cross(direction, glm::vec3(0.f, 1.f, 0.f)));
    }

    //Update engine
    mBackEngineRight->GetComponent<Component::Transform>()->pitch = pitchFactor * -35.f * velocityFactor;
    mBackEngineLeft->GetComponent<Component::Transform>()->pitch = pitchFactor * 35.f * velocityFactor;
    mFrontEngineRight->GetComponent<Component::Transform>()->pitch = pitchFactor * -35.f * velocityFactor;
    mFrontEngineLeft->GetComponent<Component::Transform>()->pitch = pitchFactor * 35.f * velocityFactor;
    mBackEngineRight->GetComponent<Component::Transform>()->roll = rollFactor * -15.f * velocityFactor;
    mBackEngineLeft->GetComponent<Component::Transform>()->roll = rollFactor * 15.f * velocityFactor;
    mFrontEngineRight->GetComponent<Component::Transform>()->roll = rollFactor * -15.f * velocityFactor;
    mFrontEngineLeft->GetComponent<Component::Transform>()->roll = rollFactor * 15.f * velocityFactor;
    mBackEngineRightParticles->GetComponent<Component::ParticleEmitter>()->particleType.minVelocity = glm::vec3(0.f, -10.f, 0.f) * (1 + velocityFactor);
    mBackEngineRightParticles->GetComponent<Component::ParticleEmitter>()->particleType.maxVelocity = glm::vec3(0.3f, -15.f, 0.3f) * (1 + velocityFactor);
    mBackEngineLeftParticles->GetComponent<Component::ParticleEmitter>()->particleType.minVelocity = glm::vec3(0.f, -10.f, 0.f) * (1 + velocityFactor);
    mBackEngineLeftParticles->GetComponent<Component::ParticleEmitter>()->particleType.maxVelocity = glm::vec3(0.3f, -15.f, 0.3f) * (1 + velocityFactor);
    mFrontEngineRightParticles->GetComponent<Component::ParticleEmitter>()->particleType.minVelocity = glm::vec3(0.f, -10.f, 0.f) * (1 + velocityFactor);
    mFrontEngineRightParticles->GetComponent<Component::ParticleEmitter>()->particleType.maxVelocity = glm::vec3(0.3f, -15.f, 0.3f) * (1 + velocityFactor);
    mFrontEngineLeftParticles->GetComponent<Component::ParticleEmitter>()->particleType.minVelocity = glm::vec3(0.f, -10.f, 0.f) * (1 + velocityFactor);
    mFrontEngineLeftParticles->GetComponent<Component::ParticleEmitter>()->particleType.maxVelocity = glm::vec3(0.3f, -15.f, 0.3f) * (1 + velocityFactor);

    // Update body
    mBody->GetComponent<Component::Transform>()->pitch = pitchFactor * 10.f * velocityFactor;
    mBody->GetComponent<Component::Transform>()->roll = rollFactor * 15.f * velocityFactor;

    // Update turrets
    float recoilFactor;
    recoilFactor = glm::min(1.f, mLeftSpawnNode->GetComponent<Component::Spawner>()->timeSinceSpawn / mLeftSpawnNode->GetComponent<Component::Spawner>()->delay);
    mLeftTurretBarrel->GetComponent<Component::Transform>()->position.z = recoilFactor * 3.f - 3.f;
    recoilFactor = glm::min(1.f, mRightSpawnNode->GetComponent<Component::Spawner>()->timeSinceSpawn / mRightSpawnNode->GetComponent<Component::Spawner>()->delay);
    mRightTurretBarrel->GetComponent<Component::Transform>()->position.z = recoilFactor * 3.f - 3.f;
}
