#include "Player2.hpp"

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

Player2::Player2(Scene* scene) : SuperPlayer(scene) {
    mActive = true;
    mRespawnTimer = 5;

    mState = LIGHTDAMAGE;

    mHealthyTexture = Resources().CreateTexture2DFromFile("Resources/player2_diff.png");
    mMediumDamageTexture = Resources().CreateTexture2DFromFile("Resources/player2_diff_medium_damage.png");
    mHeavyDamageTexture = Resources().CreateTexture2DFromFile("Resources/player2_diff_heavy_damage.png");

    mNode = CreateEntity();
    mNode->AddComponent<Component::Transform>()->scale *= 0.33f;
    mNode->AddComponent<Component::Controller>()->speed = 5000.f;
    mNode->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Move);
    mNode->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Shield);
    mNode->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::MouseRotate);
    mNode->GetComponent<Component::Controller>()->playerID = InputHandler::PLAYER_TWO;
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

    mNode->AddComponent<Component::Update>()->updateFunction = std::bind(&Player2::mUpdateFunction, this);

    mBody = CreateEntity();
    mBody->AddComponent<Component::RelativeTransform>()->parentEntity = mNode;
    mBody->AddComponent<Component::Mesh>()->geometry = mBodyModel = Resources().CreateOBJModel("Resources/player2_body.obj");
    mBody->AddComponent<Component::Material>();

    Resources().FreeTexture2D(mBody->GetComponent<Component::Material>()->diffuse);
    mBody->GetComponent<Component::Material>()->diffuse = mHealthyTexture;
    mBody->GetComponent<Component::Material>()->SetSpecular("Resources/player2_spec.png");
    mBody->GetComponent<Component::Material>()->SetGlow("Resources/player2_glow.png");
    mBody->GetComponent<Component::Material>()->SetNormal("Resources/player2_norm.png");
    mBody->AddComponent<Component::Animation>();

    mLight = CreateEntity();
    mLight->AddComponent<Component::RelativeTransform>()->Move(0, 1, 2);
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

    mTurretBodyModel = Resources().CreateOBJModel("Resources/turret_body.obj");
    mTurretBarrelModel = Resources().CreateOBJModel("Resources/turret_barrel.obj");

    // Left Turret
    mLeftTurretBody = CreateEntity();
    mLeftTurretBody->AddComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mLeftTurretBody->GetComponent<Component::RelativeTransform>()->Move(3.f, -2.5f, 5.f);
    mLeftTurretBody->GetComponent<Component::RelativeTransform>()->roll = 45.f;
    mLeftTurretBody->GetComponent<Component::RelativeTransform>()->scale *= 0.6f;
    mLeftTurretBody->AddComponent<Component::Animation>();
    mLeftTurretBody->AddComponent<Component::Mesh>()->geometry = mTurretBodyModel;
    mLeftTurretBody->AddComponent<Component::Material>()->SetDiffuse("Resources/turret_diff.png");

    mLeftTurretBarrel = CreateEntity();
    mLeftTurretBarrel->AddComponent<Component::RelativeTransform>()->parentEntity = mLeftTurretBody;
    mLeftTurretBarrel->AddComponent<Component::Animation>();
    mLeftTurretBarrel->AddComponent<Component::Mesh>()->geometry = mTurretBarrelModel;
    mLeftTurretBarrel->AddComponent<Component::Material>()->SetDiffuse("Resources/turret_diff.png");

    mLeftSpawnNode = CreateEntity();
    mLeftSpawnNode->AddComponent<Component::RelativeTransform>()->parentEntity = mLeftTurretBarrel;
    mLeftSpawnNode->GetComponent<Component::RelativeTransform>()->Move(0.f, 0.f, 10.f);
    mLeftSpawnNode->AddComponent<Component::Animation>();
    mLeftSpawnNode->AddComponent<Component::Spawner>()->delay = 0.3f;
    mLeftSpawnNode->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AimedFire);
    mLeftSpawnNode->GetComponent<Component::Controller>()->playerID = InputHandler::PLAYER_TWO;
    Component::SoundSource* sound = mLeftSpawnNode->AddComponent<Component::SoundSource>();
    mShootSound = Resources().CreateSound("Resources/Laser.ogg");
    sound->soundBuffer = mShootSound;
    sound->gain = 2.f;

    // Right Turret
    mRightTurretBody = CreateEntity();
    mRightTurretBody->AddComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mRightTurretBody->GetComponent<Component::RelativeTransform>()->Move(-3.f, -2.5f, 5.f);
    mRightTurretBody->GetComponent<Component::RelativeTransform>()->roll = -45.f;
    mRightTurretBody->GetComponent<Component::RelativeTransform>()->scale *= 0.6f;
    mRightTurretBody->AddComponent<Component::Animation>();
    mRightTurretBody->AddComponent<Component::Mesh>()->geometry = mTurretBodyModel;
    mRightTurretBody->AddComponent<Component::Material>()->SetDiffuse("Resources/turret_diff.png");

    mRightTurretBarrel = CreateEntity();
    mRightTurretBarrel->AddComponent<Component::RelativeTransform>()->parentEntity = mRightTurretBody;
    mRightTurretBarrel->AddComponent<Component::Animation>();
    mRightTurretBarrel->AddComponent<Component::Mesh>()->geometry = mTurretBarrelModel;
    mRightTurretBarrel->AddComponent<Component::Material>()->SetDiffuse("Resources/turret_diff.png");

    mRightSpawnNode = CreateEntity();
    mRightSpawnNode->AddComponent<Component::RelativeTransform>()->parentEntity = mRightTurretBarrel;
    mRightSpawnNode->GetComponent<Component::RelativeTransform>()->Move(0.f, 0.f, 10.f);
    mRightSpawnNode->AddComponent<Component::Animation>();
    mRightSpawnNode->AddComponent<Component::Spawner>()->delay = 0.3f;
    mRightSpawnNode->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AimedFire);
    mRightSpawnNode->GetComponent<Component::Controller>()->playerID = InputHandler::PLAYER_TWO;

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
}

Player2::~Player2() {
    if (mState != LIGHTDAMAGE)
        Resources().FreeTexture2D(mHealthyTexture);
    if (mState != MEDIUMDAMAGE)
        Resources().FreeTexture2D(mMediumDamageTexture);
    if (mState != HEAVYDAMAGE)
        Resources().FreeTexture2D(mHeavyDamageTexture);
    Resources().FreeSound(mShootSound);

    Resources().FreeOBJModel(mBodyModel);
    Resources().FreeOBJModel(mEngineModel);
    Resources().FreeOBJModel(mPropellerModel);
    Resources().FreeOBJModel(mTurretBarrelModel);
    Resources().FreeOBJModel(mTurretBodyModel);
}

glm::vec3 Player2::GetPosition() {
    return mNode->GetComponent<Component::Transform>()->GetWorldPosition();
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
    mNode->GetComponent<Component::Controller>()->enabled = true;
    mLeftSpawnNode->GetComponent<Component::Controller>()->enabled = true;
    mRightSpawnNode->GetComponent<Component::Controller>()->enabled = true;
    mNode->GetComponent<Component::Health>()->health = mNode->GetComponent<Component::Health>()->maxHealth;
    mNode->GetComponent<Component::ParticleEmitter>()->enabled = false;
    mNode->GetComponent<Component::Health>()->regenAmount = 20.f;
}

void Player2::Deactivate() {

    mActive = false;
    mNode->GetComponent<Component::Controller>()->enabled = false;
    mLeftSpawnNode->GetComponent<Component::Controller>()->enabled = false;
    mRightSpawnNode->GetComponent<Component::Controller>()->enabled = false;
    mNode->GetComponent<Component::ParticleEmitter>()->enabled = true;
    mNode->GetComponent<Component::Physics>()->acceleration = glm::vec3(0, 0, 0);
    mNode->GetComponent<Component::Health>()->regenAmount = 0.f;

}

void Player2::AddEngine(Entity* entity, glm::vec3 position, glm::vec3 scale) {
    entity->AddComponent<Component::RelativeTransform>()->Move(position);
    entity->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    entity->GetComponent<Component::RelativeTransform>()->scale = scale;
    entity->AddComponent<Component::Animation>();
    entity->AddComponent<Component::Mesh>()->geometry = mEngineModel;
    entity->AddComponent<Component::Material>();
    entity->GetComponent<Component::Material>()->SetDiffuse("Resources/player2_rest_diff.png");
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
}

void Player2::mUpdateFunction() {
    //Update health texture
    if (GetHealth() >= 2.f*(mNode->GetComponent<Component::Health>()->maxHealth / 3.f)) {
        mState = LIGHTDAMAGE;
        mBody->GetComponent<Component::Material>()->diffuse = mHealthyTexture;
        mLight->GetComponent<Component::SpotLight>()->color = glm::vec3(1.f, 1.f, 1.f);
        mBottomLight->GetComponent<Component::PointLight>()->color = glm::vec3(0.f, 1.f, 0.f);
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
    }

    glm::vec3 velocity = mNode->GetComponent<Component::Physics>()->velocity;

    //Update factors
    float velocityFactor = glm::length(mNode->GetComponent<Component::Physics>()->velocity) / mNode->GetComponent<Component::Physics>()->maxVelocity;
    float pitchFactor = 0.f;
    float rollFactor = 0.f;
    if (glm::length(velocity) > 0.01f) {
        pitchFactor = glm::dot(glm::normalize(velocity), mNode->GetComponent<Component::Transform>()->GetWorldDirection());
        rollFactor = glm::dot(glm::normalize(velocity), glm::cross(mNode->GetComponent<Component::Transform>()->GetWorldDirection(), glm::vec3(0.f, 1.f, 0.f)));
    }

    //Update midpropeller
    mMidPropeller->GetComponent<Component::Physics>()->angularVelocity.y = velocityFactor * 2.f + 0.2f;

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

    // Update turrets
    float recoilFactor;
    recoilFactor = glm::min(1.f, mLeftSpawnNode->GetComponent<Component::Spawner>()->timeSinceSpawn / mLeftSpawnNode->GetComponent<Component::Spawner>()->delay);
    mLeftTurretBarrel->GetComponent<Component::Transform>()->position.z = recoilFactor * 3.f - 3.f;
    recoilFactor = glm::min(1.f, mRightSpawnNode->GetComponent<Component::Spawner>()->timeSinceSpawn / mRightSpawnNode->GetComponent<Component::Spawner>()->delay);
    mRightTurretBarrel->GetComponent<Component::Transform>()->position.z = recoilFactor * 3.f - 3.f;
}
