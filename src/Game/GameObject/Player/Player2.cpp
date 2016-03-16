#include "Player2.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>

#include <Resources.hpp>
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

Player2::Player2(Scene* scene) : SuperPlayer(scene) {
    mHealthyTexture = Resources().CreateTexture2DFromFile("Resources/player2_diff.png");
    mMediumDamageTexture = Resources().CreateTexture2DFromFile("Resources/player2_diff_medium_damage.png");
    mHeavyDamageTexture = Resources().CreateTexture2DFromFile("Resources/player2_diff_heavy_damage.png");
    mDeadTexture = Resources().CreateTexture2DFromFile("Resources/player2_diff_dead.png");
    mCollisionRadius = 10.f;

    mNode = CreateEntity();
    mNode->AddComponent<Component::Transform>()->scale *= 0.33f; //0.25f
    mNode->AddComponent<Component::Controller>()->speed = 5000.f;
    mNode->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Move);
    mNode->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Shield);
    mNode->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Aim);
    mNode->GetComponent<Component::Controller>()->playerID = InputHandler::PLAYER_ONE;
    mNode->AddComponent<Component::Physics>()->velocityDragFactor = 3.f;
    mNode->AddComponent<Component::Health>()->removeOnLowHealth = false;
    mNode->GetComponent<Component::Health>()->health = mNode->GetComponent<Component::Health>()->maxHealth = 30.f;
    mNode->GetComponent<Component::Health>()->maxCooldown = 3.f;
    //Regain full health after 5 seconds.
    mNode->GetComponent<Component::Health>()->regainAmount = mRegainAmount = mNode->GetComponent<Component::Health>()->maxHealth / 5.f;
    mNode->GetComponent<Component::Health>()->faction = 0;
    mNode->AddComponent<Component::Collider2DCircle>()->radius = mCollisionRadius;
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

    mNode->AddComponent<Component::Update>()->updateFunction = std::bind(&Player2::mUpdateFunction, this);

    mBody = CreateEntity();
    mBody->AddComponent<Component::RelativeTransform>()->parentEntity = mNode;
    mBody->AddComponent<Component::Mesh>()->geometry = mBodyModel = Resources().CreateOBJModel("Resources/player2_body.obj");
    mBody->AddComponent<Component::Material>();

    Resources().FreeTexture2D(mBody->GetComponent<Component::Material>()->diffuse);
    mBody->GetComponent<Component::Material>()->diffuse = mHealthyTexture;
    mBody->GetComponent<Component::Material>()->SetSpecular("Resources/player2_spec.png");
    mBody->GetComponent<Component::Material>()->SetGlow("Resources/player2_glow.png");
    mBody->AddComponent<Component::Animation>();

    mLight = CreateEntity();
    mLight->AddComponent<Component::RelativeTransform>()->Move(0, 1, 2);
    mLight->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mLight->GetComponent<Component::RelativeTransform>()->pitch = 15.f;
    mLight->AddComponent<Component::Animation>();
    mLight->AddComponent<Component::SpotLight>()->coneAngle = 20.f;
    mLight->GetComponent<Component::SpotLight>()->attenuation = 0.1f;
    mLight->GetComponent<Component::SpotLight>()->intensity = 3.f;

    mBottomLight = CreateEntity();
    mBottomLight->AddComponent<Component::RelativeTransform>()->Move(0.f, -7.f, 0.f);
    mBottomLight->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mBottomLight->AddComponent<Component::Animation>();
    mBottomLight->AddComponent<Component::PointLight>();
    mBottomLight->GetComponent<Component::PointLight>()->color = glm::vec3(1.f, 1.f, 1.f);
    mBottomLight->GetComponent<Component::PointLight>()->attenuation = 0.8f;
    mBottomLight->GetComponent<Component::PointLight>()->intensity = 0.f;

    mTurretBodyModel = Resources().CreateOBJModel("Resources/turret_body.obj");
    mTurretBarrelModel = Resources().CreateOBJModel("Resources/turret_barrel.obj");

    // Left Turret
    mLeftTurretBody = CreateEntity();
    mLeftTurretBody->AddComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mLeftTurretBody->GetComponent<Component::RelativeTransform>()->Move(3.f, -2.5f, 6.f);
    mLeftTurretBody->GetComponent<Component::RelativeTransform>()->roll = 45.f;
    mLeftTurretBody->GetComponent<Component::RelativeTransform>()->scale *= 0.6f;
    mLeftTurretBody->AddComponent<Component::Animation>();
    mLeftTurretBody->AddComponent<Component::Mesh>()->geometry = mTurretBodyModel;
    mLeftTurretBody->AddComponent<Component::Material>()->SetDiffuse("Resources/turret_diff.png");
    mLeftTurretBody->GetComponent<Component::Material>()->SetSpecular("Resources/turret_spec.png");

    mLeftTurretBarrel.node = CreateEntity();
    mLeftTurretBarrel.node->AddComponent<Component::RelativeTransform>()->parentEntity = mLeftTurretBody;
    mLeftTurretBarrel.node->GetComponent<Component::RelativeTransform>()->Move(0.f, 0.f, -1.5f);
    CreateBarrel(&mLeftTurretBarrel);

    mLeftSpawnNode = CreateEntity();
    mLeftSpawnNode->AddComponent<Component::RelativeTransform>()->parentEntity = mLeftTurretBarrel.node;
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
    mRightTurretBody->GetComponent<Component::RelativeTransform>()->Move(-3.f, -2.5f, 6.f);
    mRightTurretBody->GetComponent<Component::RelativeTransform>()->roll = -45.f;
    mRightTurretBody->GetComponent<Component::RelativeTransform>()->scale *= 0.6f;
    mRightTurretBody->AddComponent<Component::Animation>();
    mRightTurretBody->AddComponent<Component::Mesh>()->geometry = mTurretBodyModel;
    mRightTurretBody->AddComponent<Component::Material>()->SetDiffuse("Resources/turret_diff.png");
    mRightTurretBody->GetComponent<Component::Material>()->SetSpecular("Resources/turret_spec.png");

    mRightTurretBarrel.node = CreateEntity();
    mRightTurretBarrel.node->AddComponent<Component::RelativeTransform>()->parentEntity = mRightTurretBody;
    mRightTurretBarrel.node->GetComponent<Component::RelativeTransform>()->Move(0.f, 0.f, -1.5f);
    CreateBarrel(&mRightTurretBarrel);

    mRightSpawnNode = CreateEntity();
    mRightSpawnNode->AddComponent<Component::RelativeTransform>()->parentEntity = mRightTurretBarrel.node;
    mRightSpawnNode->GetComponent<Component::RelativeTransform>()->Move(0.f, 0.f, 10.f);
    mRightSpawnNode->AddComponent<Component::Animation>();
    mRightSpawnNode->AddComponent<Component::Spawner>()->delay = 0.3f;
    mRightSpawnNode->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AimedFire);
    mRightSpawnNode->GetComponent<Component::Controller>()->playerID = InputHandler::PLAYER_ONE;

    mEngineModel = Resources().CreateOBJModel("Resources/player2_engine.obj");
    mPropellerModel = Resources().CreateOBJModel("Resources/player2_propeller.obj");


    mMidPropeller = CreateEntity();
    AddMidPropeller(mMidPropeller, glm::vec3(0.f, 1.f, -3.3f), glm::vec3(1.f, 1.f, 1.f) * 3.7f);

    Entity* pointLight;

    mFrontEngineLeft = CreateEntity();
    AddEngine(mFrontEngineLeft, glm::vec3(5.5f, -1.2f, 5.0f), glm::vec3(1.f, 1.f, 1.f) * 0.6f);
    mFrontPropellerLeft = CreateEntity();
    mFrontPropellerLeft->AddComponent<Component::RelativeTransform>()->parentEntity = mFrontEngineLeft;
    AddPropeller(mFrontPropellerLeft, glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f) * 1.f);
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
    AddEngine(mFrontEngineRight, glm::vec3(-5.5f, -1.2f, 5.0f), glm::vec3(1.f, 1.f, 1.f) * 0.6f);
    mFrontPropellerRight = CreateEntity();
    mFrontPropellerRight->AddComponent<Component::RelativeTransform>()->parentEntity = mFrontEngineRight;
    AddPropeller(mFrontPropellerRight, glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f) * 1.f);
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
    AddEngine(mBackEngineLeft, glm::vec3(7.f, -0.9f, -0.5f), glm::vec3(1.f, 1.f, 1.f) * 1.f);
    mBackPropellerLeft = CreateEntity();
    mBackPropellerLeft->AddComponent<Component::RelativeTransform>()->parentEntity = mBackEngineLeft;
    AddPropeller(mBackPropellerLeft, glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f) * 1.f);
    //pointLight = CreateEntity();
    //pointLight->AddComponent<Component::PointLight>();
    //pointLight->GetComponent<Component::PointLight>()->attenuation = 5.f;
    //pointLight->AddComponent<Component::RelativeTransform>()->parentEntity = mBackEngineLeft;
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
    AddEngine(mBackEngineRight, glm::vec3(-7.f, -0.9f, -0.5f), glm::vec3(1.f, 1.f, 1.f) * 1.f);
    mBackPropellerRight = CreateEntity();
    mBackPropellerRight->AddComponent<Component::RelativeTransform>()->parentEntity = mBackEngineRight;
    AddPropeller(mBackPropellerRight, glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f) * 1.f);
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
    
    mUpdateFunction();
}

Player2::~Player2() {
    if (mState != LIGHTDAMAGE)
        Resources().FreeTexture2D(mHealthyTexture);
    if (mState != MEDIUMDAMAGE)
        Resources().FreeTexture2D(mMediumDamageTexture);
    if (mState != HEAVYDAMAGE)
        Resources().FreeTexture2D(mHeavyDamageTexture);
    if (mState != DEAD)
        Resources().FreeTexture2D(mDeadTexture);
    Resources().FreeSound(mShootSound);

    Resources().FreeOBJModel(mBodyModel);
    Resources().FreeOBJModel(mEngineModel);
    Resources().FreeOBJModel(mPropellerModel);
    Resources().FreeOBJModel(mTurretBarrelModel);
    Resources().FreeOBJModel(mTurretBodyModel);
}

glm::vec3 Player2::GetPosition() {
    return mNode->GetComponent<Component::Transform>()->position;
}

void Player2::SetPosition(glm::vec3 position) {
    mNode->GetComponent<Component::Transform>()->position = position;
}

Entity* Player2::GetNodeEntity() {
    return mNode;
}


float Player2::GetHealth() {
    return mNode->GetComponent<Component::Health>()->health;
}

void Player2::Activate() {

    mActive = true;
    mNode->AddComponent<Component::Collider2DCircle>()->radius = mCollisionRadius;
    mNode->GetComponent<Component::Controller>()->enabled = true;
    mLeftSpawnNode->GetComponent<Component::Controller>()->enabled = true;
    mRightSpawnNode->GetComponent<Component::Controller>()->enabled = true;
    mNode->GetComponent<Component::Health>()->health = mNode->GetComponent<Component::Health>()->maxHealth;
    mNode->GetComponent<Component::ParticleEmitter>()->enabled = false;
    mNode->GetComponent<Component::Health>()->regainAmount = mRegainAmount;
}

void Player2::Deactivate() {

    mActive = false;
    mNode->KillComponent<Component::Collider2DCircle>();
    mNode->GetComponent<Component::Controller>()->enabled = false;
    mLeftSpawnNode->GetComponent<Component::Controller>()->enabled = false;
    mRightSpawnNode->GetComponent<Component::Controller>()->enabled = false;
    mNode->GetComponent<Component::ParticleEmitter>()->enabled = true;
    mNode->GetComponent<Component::Physics>()->acceleration = glm::vec3(0, 0, 0);
    mNode->GetComponent<Component::Health>()->regainAmount = 0.f;

}

void Player2::AddEngine(Entity* entity, glm::vec3 position, glm::vec3 scale) {
    entity->AddComponent<Component::RelativeTransform>()->Move(position);
    entity->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    entity->GetComponent<Component::RelativeTransform>()->scale = scale;
    entity->AddComponent<Component::Animation>();
    entity->AddComponent<Component::Mesh>()->geometry = mEngineModel;
    entity->AddComponent<Component::Material>();
    entity->GetComponent<Component::Material>()->SetDiffuse("Resources/player2_rest_diff.png");
    entity->GetComponent<Component::Material>()->SetSpecular("Resources/player2_rest_spec.png");
}

void Player2::AddMidPropeller(Entity* entity, glm::vec3 position, glm::vec3 scale) {
    entity->AddComponent<Component::RelativeTransform>()->parentEntity = mBody;
    entity->GetComponent<Component::RelativeTransform>()->Move(position);
    entity->GetComponent<Component::RelativeTransform>()->scale = scale;
    entity->AddComponent<Component::Physics>()->angularDragFactor = 0.f;
    entity->GetComponent<Component::Physics>()->angularVelocity.y = 2.f * (entity->GetComponent<Component::RelativeTransform>()->parentEntity->GetComponent<Component::Transform>()->position.x < 0.f) * 2.f - 1.f;
    entity->AddComponent<Component::Animation>();
    entity->AddComponent<Component::Mesh>()->geometry = mPropellerModel;
    entity->AddComponent<Component::Material>();
    entity->GetComponent<Component::Material>()->SetDiffuse("Resources/player2_rest_diff.png");
    entity->GetComponent<Component::Material>()->SetSpecular("Resources/player2_rest_spec.png");

    for (int i = 0; i < 4; i++) {
        mMidPropellerParticles[i] = CreateEntity();
        Component::RelativeTransform* transform = mMidPropellerParticles[i]->AddComponent<Component::RelativeTransform>();
        transform->parentEntity = entity;
        transform->yaw = i * 360.f / 4;
        transform->position = transform->GetWorldDirection() + glm::vec3(0.f, 0.25f, 0.f);
        mMidPropellerParticles[i]->AddComponent<Component::Animation>();
        Component::ParticleEmitter* emitter = mMidPropellerParticles[i]->AddComponent<Component::ParticleEmitter>();
        emitter->emitterType = Component::ParticleEmitter::POINT;
        emitter->maxEmitTime = emitter->minEmitTime = 0.0016;
        emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
        emitter->lifetime = 0.0;
        emitter->particleType.textureIndex = Component::ParticleEmitter::BLUE;
        emitter->particleType.minLifetime = 0.01f * 3.f;
        emitter->particleType.maxLifetime = 0.01f * 3.f;
        emitter->particleType.minVelocity = glm::vec3(0.f, 0.f, 0.f);
        emitter->particleType.maxVelocity = glm::vec3(0.f, 0.f, 0.f);
        emitter->particleType.minSize = glm::vec2(.5f, .5f);
        emitter->particleType.maxSize = glm::vec2(.7f, .7f);
        emitter->particleType.uniformScaling = true;
        emitter->particleType.color = glm::vec3(.8f, .8f, .8f);
        emitter->particleType.startAlpha = 1.f;
        emitter->particleType.midAlpha = 1.f;
        emitter->particleType.endAlpha = 0.f;
    }
}

void Player2::AddPropeller(Entity* entity, glm::vec3 position, glm::vec3 scale) {
    entity->GetComponent<Component::RelativeTransform>()->Move(position);
    entity->GetComponent<Component::RelativeTransform>()->scale = scale;
    entity->AddComponent<Component::Physics>()->angularDragFactor = 0.f;
    entity->GetComponent<Component::Physics>()->angularVelocity.y = 2.f * (entity->GetComponent<Component::RelativeTransform>()->parentEntity->GetComponent<Component::Transform>()->position.x < 0.f) * 2.f - 1.f;
    entity->AddComponent<Component::Animation>();
    entity->AddComponent<Component::Mesh>()->geometry = mPropellerModel;
    entity->AddComponent<Component::Material>();
    entity->GetComponent<Component::Material>()->SetDiffuse("Resources/player2_rest_diff.png");
    entity->GetComponent<Component::Material>()->SetSpecular("Resources/player2_rest_spec.png");
}

void Player2::CreateBarrel(Barrel* barrel) {
    barrel->node->AddComponent<Component::Animation>();

    barrel->barrel[0] = CreateEntity();
    barrel->barrel[0]->AddComponent<Component::RelativeTransform>()->parentEntity = barrel->node;
    barrel->barrel[0]->GetComponent<Component::Transform>()->Move(1.f, 0.f, 0.f);
    barrel->barrel[0]->AddComponent<Component::Mesh>()->geometry = mTurretBarrelModel;
    barrel->barrel[0]->AddComponent<Component::Material>()->SetDiffuse("Resources/turret_diff.png");
    barrel->barrel[0]->AddComponent<Component::Animation>();

    barrel->barrel[1] = CreateEntity();
    barrel->barrel[1]->AddComponent<Component::RelativeTransform>()->parentEntity = barrel->node;
    barrel->barrel[1]->GetComponent<Component::Transform>()->Move(-1.f, 0.f, 0.f);
    barrel->barrel[1]->AddComponent<Component::Mesh>()->geometry = mTurretBarrelModel;
    barrel->barrel[1]->AddComponent<Component::Material>()->SetDiffuse("Resources/turret_diff.png");
    barrel->barrel[1]->AddComponent<Component::Animation>();
}

void Player2::mUpdateFunction() {

    //Update health texture
    if (GetHealth() >= 2.f*(mNode->GetComponent<Component::Health>()->maxHealth / 3.f)) {
        mState = LIGHTDAMAGE;
        mBody->GetComponent<Component::Material>()->diffuse = mHealthyTexture;
        mLight->GetComponent<Component::SpotLight>()->color = glm::vec3(0.f, 1.f, 0.f);
        mBottomLight->GetComponent<Component::PointLight>()->color = glm::vec3(0.f, 1.f, 0.f);
        mFrontEngineLeft->GetComponent<Component::ParticleEmitter>()->enabled = false;
        mFrontEngineRight->GetComponent<Component::ParticleEmitter>()->enabled = false;
        mBackEngineLeft->GetComponent<Component::ParticleEmitter>()->enabled = false;
        mBackEngineRight->GetComponent<Component::ParticleEmitter>()->enabled = false;
        mMidPropellerParticles[0]->GetComponent<Component::ParticleEmitter>()->enabled = true;
        mMidPropellerParticles[1]->GetComponent<Component::ParticleEmitter>()->enabled = true;
        mMidPropellerParticles[2]->GetComponent<Component::ParticleEmitter>()->enabled = true;
        mMidPropellerParticles[3]->GetComponent<Component::ParticleEmitter>()->enabled = true;

    } else if (GetHealth() >= 1.f*(mNode->GetComponent<Component::Health>()->maxHealth / 3.f)) {
        mBottomLight->GetComponent<Component::PointLight>()->intensity = 3.f;
        mState = MEDIUMDAMAGE;
        mLight->GetComponent<Component::SpotLight>()->color = glm::vec3(1.f, 1.0f, 0.0f);
        mBottomLight->GetComponent<Component::PointLight>()->color = glm::vec3(1.f, 1.f, 0.f);
        mBody->GetComponent<Component::Material>()->diffuse = mMediumDamageTexture;
        mFrontEngineLeft->GetComponent<Component::ParticleEmitter>()->enabled = false;
        mFrontEngineRight->GetComponent<Component::ParticleEmitter>()->enabled = true;
        mBackEngineLeft->GetComponent<Component::ParticleEmitter>()->enabled = true;
        mBackEngineRight->GetComponent<Component::ParticleEmitter>()->enabled = false;
        mMidPropellerParticles[0]->GetComponent<Component::ParticleEmitter>()->enabled = false;
        mMidPropellerParticles[1]->GetComponent<Component::ParticleEmitter>()->enabled = true;
        mMidPropellerParticles[2]->GetComponent<Component::ParticleEmitter>()->enabled = true;
        mMidPropellerParticles[3]->GetComponent<Component::ParticleEmitter>()->enabled = true;
    } else if (GetHealth() >= 0.01f) {
        mState = HEAVYDAMAGE;
        mLight->GetComponent<Component::SpotLight>()->color = glm::vec3(1.f, 0.0f, 0.0f);
        mBottomLight->GetComponent<Component::PointLight>()->color = glm::vec3(1.f, 0.32f, 0.f);
        mBody->GetComponent<Component::Material>()->diffuse = mHeavyDamageTexture;
        mFrontEngineLeft->GetComponent<Component::ParticleEmitter>()->enabled = true;
        mFrontEngineRight->GetComponent<Component::ParticleEmitter>()->enabled = true;
        mBackEngineLeft->GetComponent<Component::ParticleEmitter>()->enabled = true;
        mBackEngineRight->GetComponent<Component::ParticleEmitter>()->enabled = false;
        mMidPropellerParticles[0]->GetComponent<Component::ParticleEmitter>()->enabled = false;
        mMidPropellerParticles[1]->GetComponent<Component::ParticleEmitter>()->enabled = false;
        mMidPropellerParticles[2]->GetComponent<Component::ParticleEmitter>()->enabled = true;
        mMidPropellerParticles[3]->GetComponent<Component::ParticleEmitter>()->enabled = true;
    } else {
         mState = DEAD;
         mLight->GetComponent<Component::SpotLight>()->color = glm::vec3(1.f, 0.0f, 0.0f);
         mBottomLight->GetComponent<Component::PointLight>()->color = glm::vec3(1.f, 0.f, 0.f);
         mBody->GetComponent<Component::Material>()->diffuse = mDeadTexture;
         mFrontEngineLeft->GetComponent<Component::ParticleEmitter>()->enabled = true;
         mFrontEngineRight->GetComponent<Component::ParticleEmitter>()->enabled = true;
         mBackEngineLeft->GetComponent<Component::ParticleEmitter>()->enabled = true;
         mBackEngineRight->GetComponent<Component::ParticleEmitter>()->enabled = true;
         mMidPropellerParticles[0]->GetComponent<Component::ParticleEmitter>()->enabled = false;
         mMidPropellerParticles[1]->GetComponent<Component::ParticleEmitter>()->enabled = false;
         mMidPropellerParticles[2]->GetComponent<Component::ParticleEmitter>()->enabled = false;
         mMidPropellerParticles[3]->GetComponent<Component::ParticleEmitter>()->enabled = true;
 }

    glm::vec3 velocity = mNode->GetComponent<Component::Physics>()->velocity;

    //Update factors
    float velocityFactor = glm::length(mNode->GetComponent<Component::Physics>()->velocity) / mNode->GetComponent<Component::Physics>()->maxVelocity;
    float pitchFactor = 0.f;
    float rollFactor = 0.f;
    glm::vec3 frontDirection = mNode->GetComponent<Component::Transform>()->GetWorldDirection();
    glm::vec3 sideDirection = glm::cross(frontDirection, glm::vec3(0.f, 1.f, 0.f));
    if (glm::length(velocity) > 0.01f) {
        pitchFactor = glm::dot(glm::normalize(velocity), frontDirection);
        rollFactor = glm::dot(glm::normalize(velocity), sideDirection);
    }

    //Update propeller
    mBackPropellerRight->GetComponent<Component::Physics>()->angularVelocity.y = velocityFactor * 2.f + 1.2f;
    mBackPropellerLeft->GetComponent<Component::Physics>()->angularVelocity.y = velocityFactor * 2.f + 1.2f;
    mFrontPropellerRight->GetComponent<Component::Physics>()->angularVelocity.y = velocityFactor * 2.f + 1.2f;
    mFrontPropellerLeft->GetComponent<Component::Physics>()->angularVelocity.y = velocityFactor * 2.f + 1.2f;

    //Update engine
    mBackEngineRight->GetComponent<Component::Transform>()->pitch = pitchFactor * 35.f * velocityFactor;
    mBackEngineLeft->GetComponent<Component::Transform>()->pitch = pitchFactor * 35.f * velocityFactor;
    mFrontEngineRight->GetComponent<Component::Transform>()->pitch = pitchFactor * 30.f * velocityFactor;
    mFrontEngineLeft->GetComponent<Component::Transform>()->pitch = pitchFactor * 30.f * velocityFactor;
    mBackEngineRight->GetComponent<Component::Transform>()->roll = rollFactor * 15.f * velocityFactor;
    mBackEngineLeft->GetComponent<Component::Transform>()->roll = rollFactor * 15.f * velocityFactor;
    mFrontEngineRight->GetComponent<Component::Transform>()->roll = rollFactor * 15.f * velocityFactor;
    mFrontEngineLeft->GetComponent<Component::Transform>()->roll = rollFactor * 15.f * velocityFactor;

    //Update body
    mBody->GetComponent<Component::Transform>()->pitch = pitchFactor * 10.f * velocityFactor;
    mBody->GetComponent<Component::Transform>()->roll = rollFactor * 25.f * velocityFactor;

    //Update midpropeller
    mMidPropeller->GetComponent<Component::Physics>()->angularVelocity.y = velocityFactor * 1.2f + 0.6f;

    // Update turrets
    float recoilFactor; //[0,1]
    float factor = 3.f;
    recoilFactor = glm::min(1.f, mLeftSpawnNode->GetComponent<Component::Spawner>()->timeSinceSpawn / mLeftSpawnNode->GetComponent<Component::Spawner>()->delay);
    mLeftTurretBarrel.barrel[0]->GetComponent<Component::Transform>()->position.z = (0.5f * recoilFactor + 0.5f) * factor - factor; //[0.5,1]
    mLeftTurretBarrel.barrel[1]->GetComponent<Component::Transform>()->position.z = (recoilFactor / 2.f) * factor - factor; //[0,0.5]
    mLeftTurretBarrel.node->GetComponent<Component::Transform>()->roll = -180 * recoilFactor;

    recoilFactor = glm::min(1.f, mRightSpawnNode->GetComponent<Component::Spawner>()->timeSinceSpawn / mRightSpawnNode->GetComponent<Component::Spawner>()->delay);
    mRightTurretBarrel.barrel[1]->GetComponent<Component::Transform>()->position.z = (0.5f * recoilFactor + 0.5f) * factor - factor; //[0.5,1]
    mRightTurretBarrel.barrel[0]->GetComponent<Component::Transform>()->position.z = (recoilFactor / 2.f) * factor - factor; //[0,0.5]
    mRightTurretBarrel.node->GetComponent<Component::Transform>()->roll = 180 * recoilFactor;
}

void Player2::SetYaw(float yaw) {
    mNode->GetComponent<Component::Transform>()->yaw = yaw;
}
