#include "Player1.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Engine/Geometry/Geometry3D.hpp>
#include <Engine/Geometry/OBJModel.hpp>

#include "../../Component/Controller.hpp"
#include "../../Component/Health.hpp"
#include "../../Component/Spawner.hpp"
#include "../../Component/Update.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/Collider2DCircle.hpp>
#include <Engine/Component/SpotLight.hpp>
#include <Engine/Component/PointLight.hpp>
#include <Engine/Component/Animation.hpp>
#include <Engine/Component/ParticleEmitter.hpp>
#include <Engine/Component/SoundSource.hpp>

#include "../../Util/ControlSchemes.hpp"

using namespace GameObject;

Player1::Player1(Scene* scene) : SuperPlayer(scene) {
    mHealthyTexture = Resources().CreateTexture2DFromFile("Resources/player1/player1_body_diff_healthy.png");
    mMediumDamageTexture = Resources().CreateTexture2DFromFile("Resources/player1/player1_body_diff_medium_damage.png");
    mHeavyDamageTexture = Resources().CreateTexture2DFromFile("Resources/player1/player1_body_diff_heavy_damage.png");
    mDeadTexture = Resources().CreateTexture2DFromFile("Resources/player1/player1_body_diff_dead.png");
    mCollisionRadius = 10.f;

    mNode = CreateEntity();
    mNode->AddComponent<Component::Transform>()->scale *= 0.2f; //0.15f
    mNode->AddComponent<Component::Controller>()->speed = 80.f;
    mNode->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Move);
    mNode->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Shield);
    mNode->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Aim);
    mNode->GetComponent<Component::Controller>()->playerID = InputHandler::PLAYER_TWO;
    mNode->GetComponent<Component::Controller>()->device = InputHandler::JOYSTICK;
    mNode->AddComponent<Component::Physics>()->velocityDragFactor = 3.f;
    mNode->AddComponent<Component::Health>()->removeOnLowHealth = false;
    mNode->GetComponent<Component::Health>()->health = mNode->GetComponent<Component::Health>()->maxHealth = 30.f;
    mNode->GetComponent<Component::Health>()->maxCooldown = 1.f;
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

    mNode->AddComponent<Component::Update>()->updateFunction = std::bind(&Player1::mUpdateFunction, this);

    mBody = CreateEntity();
    mBody->AddComponent<Component::RelativeTransform>()->parentEntity = mNode;
    mBody->AddComponent<Component::Mesh>()->geometry = mBodyModel = Resources().CreateOBJModel("Resources/player1/player1_body.obj");
    mBody->AddComponent<Component::Material>();

    Resources().FreeTexture2D(mBody->GetComponent<Component::Material>()->diffuse);
    mBody->GetComponent<Component::Material>()->diffuse = mHealthyTexture;
    mBody->GetComponent<Component::Material>()->SetSpecular("Resources/player1/player1_spec.png");
    mBody->GetComponent<Component::Material>()->SetGlow("Resources/player1/player1_body_glow.png");
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
    mBottomLight->GetComponent<Component::PointLight>()->intensity = 3.f;
    
    mTurretBodyModel = Resources().CreateOBJModel("Resources/turret/turret_body.obj");
    mTurretBarrelModel = Resources().CreateOBJModel("Resources/turret/turret_barrel.obj");

    // Left Turret
    mLeftTurretBody = CreateEntity();
    mLeftTurretBody->AddComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mLeftTurretBody->GetComponent<Component::RelativeTransform>()->Move(5.f, -5.f, 10.f);
    mLeftTurretBody->GetComponent<Component::RelativeTransform>()->roll = -45.f;
    mLeftTurretBody->GetComponent<Component::RelativeTransform>()->scale *= 1.f;
    mLeftTurretBody->AddComponent<Component::Animation>();
    mLeftTurretBody->AddComponent<Component::Mesh>()->geometry = mTurretBodyModel;
    mLeftTurretBody->AddComponent<Component::Material>()->SetDiffuse("Resources/turret/turret_diff.png");
    mLeftTurretBody->GetComponent<Component::Material>()->SetSpecular("Resources/turret/turret_spec.png");

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
    mLeftSpawnNode->GetComponent<Component::Controller>()->playerID = InputHandler::PLAYER_TWO;
    Component::SoundSource* sound = mLeftSpawnNode->AddComponent<Component::SoundSource>();
    mShootSound = Resources().CreateSound("Resources/sound/Laser.ogg");
    sound->soundBuffer = mShootSound;
    sound->gain = 2.f;

    // Right Turret
    mRightTurretBody = CreateEntity();
    mRightTurretBody->AddComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mRightTurretBody->GetComponent<Component::RelativeTransform>()->Move(-5.f, -5.f, 10.f);
    mRightTurretBody->GetComponent<Component::RelativeTransform>()->roll = 45.f;
    mRightTurretBody->GetComponent<Component::RelativeTransform>()->scale *= 1.f;
    mRightTurretBody->AddComponent<Component::Animation>();
    mRightTurretBody->AddComponent<Component::Mesh>()->geometry = mTurretBodyModel;
    mRightTurretBody->AddComponent<Component::Material>()->SetDiffuse("Resources/turret/turret_diff.png");
    mRightTurretBody->GetComponent<Component::Material>()->SetSpecular("Resources/turret/turret_spec.png");

    mRightTurretBarrel.node = CreateEntity();
    mRightTurretBarrel.node->AddComponent<Component::RelativeTransform>()->parentEntity = mRightTurretBody;
    mRightTurretBarrel.node->GetComponent<Component::RelativeTransform>()->Move(0.f, 0.f, -0.5f);
    CreateBarrel(&mRightTurretBarrel);

    mRightSpawnNode = CreateEntity();
    mRightSpawnNode->AddComponent<Component::RelativeTransform>()->parentEntity = mRightTurretBarrel.node;
    mRightSpawnNode->GetComponent<Component::RelativeTransform>()->Move(0.f, 0.f, 10.f);
    mRightSpawnNode->AddComponent<Component::Animation>();
    mRightSpawnNode->AddComponent<Component::Spawner>()->delay = 0.3f;
    mRightSpawnNode->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AimedFire);
    mRightSpawnNode->GetComponent<Component::Controller>()->playerID = InputHandler::PLAYER_TWO;

    Entity* pointLight;
    // Engine
    mFrontEngineLeft = CreateEntity();
    mFrontEngineLeft->AddComponent<Component::RelativeTransform>()->Move(13.f, -1.f, 6.5f);
    mFrontEngineLeft->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mFrontEngineLeft->GetComponent<Component::RelativeTransform>()->scale *= 1.8f;
    mFrontEngineLeft->AddComponent<Component::Animation>();
    mFrontEngineLeft->AddComponent<Component::Mesh>()->geometry = mFrontEngineModel = Resources().CreateOBJModel("Resources/player1/player1_frontEngine.obj");
    mFrontEngineLeft->AddComponent<Component::Material>();
    mFrontEngineLeft->GetComponent<Component::Material>()->SetDiffuse("Resources/player1/player1_frontEngine_diff.png");
    mFrontEngineLeft->GetComponent<Component::Material>()->SetSpecular("Resources/player1/player1_spec.png");
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
    mFrontEngineRight->AddComponent<Component::RelativeTransform>()->Move(-13.f, -1.f, 6.5f);
    mFrontEngineRight->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mFrontEngineRight->GetComponent<Component::RelativeTransform>()->scale *= 1.8f;
    mFrontEngineRight->GetComponent<Component::RelativeTransform>()->yaw = 180.f;
    mFrontEngineRight->AddComponent<Component::Animation>();
    mFrontEngineRight->AddComponent<Component::Mesh>()->geometry = mFrontEngineModel;
    mFrontEngineRight->AddComponent<Component::Material>();
    mFrontEngineRight->GetComponent<Component::Material>()->SetDiffuse("Resources/player1/player1_frontEngine_diff.png");
    mFrontEngineRight->GetComponent<Component::Material>()->SetSpecular("Resources/player1/player1_spec.png");
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
    mBackEngineLeft->AddComponent<Component::RelativeTransform>()->Move(15.5f, -2.f, -3.f);
    mBackEngineLeft->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mBackEngineLeft->GetComponent<Component::RelativeTransform>()->scale *= 1.9f;
    mBackEngineLeft->AddComponent<Component::Animation>();
    mBackEngineLeft->AddComponent<Component::Mesh>()->geometry = mBackEngineModel = Resources().CreateOBJModel("Resources/player1/player1_backEngine.obj");
    mBackEngineLeft->AddComponent<Component::Material>();
    mBackEngineLeft->GetComponent<Component::Material>()->SetDiffuse("Resources/player1/player1_backEngine_diff.png");
    mBackEngineLeft->GetComponent<Component::Material>()->SetSpecular("Resources/player1/player1_spec.png");
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
    mBackEngineRight->AddComponent<Component::RelativeTransform>()->Move(-15.5f, -2.f, -3.f);
    mBackEngineRight->GetComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mBackEngineRight->GetComponent<Component::RelativeTransform>()->scale *= 1.9f;
    mBackEngineRight->GetComponent<Component::RelativeTransform>()->yaw = 180.f;
    mBackEngineRight->AddComponent<Component::Animation>();
    mBackEngineRight->AddComponent<Component::Mesh>()->geometry = mBackEngineModel;
    mBackEngineRight->AddComponent<Component::Material>();
    mBackEngineRight->GetComponent<Component::Material>()->SetDiffuse("Resources/player1/player1_backEngine_diff.png");
    mBackEngineRight->GetComponent<Component::Material>()->SetSpecular("Resources/player1/player1_spec.png");
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
    
    mUpdateFunction();
}

Player1::~Player1() {
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
    Resources().FreeOBJModel(mFrontEngineModel);
    Resources().FreeOBJModel(mBackEngineModel);
    Resources().FreeOBJModel(mTurretBarrelModel);
    Resources().FreeOBJModel(mTurretBodyModel);
}

glm::vec3 Player1::GetPosition() {
    return mNode->GetComponent<Component::Transform>()->position;
}

void Player1::SetPosition(glm::vec3 position) {
    mNode->GetComponent<Component::Transform>()->position = position;
}

Entity* Player1::GetNodeEntity() {
    return mNode;
}

Entity* Player1::GetBodyEntity() {
    return mBody;
}

float Player1::GetHealth() {
    return mNode->GetComponent<Component::Health>()->health;
}

void Player1::Activate() {

    mActive = true;
    mNode->AddComponent<Component::Collider2DCircle>()->radius = mCollisionRadius;
    mNode->GetComponent<Component::Controller>()->enabled = true;
    mLeftSpawnNode->GetComponent<Component::Controller>()->enabled = true;
    mRightSpawnNode->GetComponent<Component::Controller>()->enabled = true;
    mNode->GetComponent<Component::Health>()->health = mNode->GetComponent<Component::Health>()->maxHealth;
    mNode->GetComponent<Component::ParticleEmitter>()->enabled = false;
    mNode->GetComponent<Component::Health>()->regainAmount = mRegainAmount;
}

void Player1::Deactivate() {

    mActive = false;
    mNode->KillComponent<Component::Collider2DCircle>();
    mNode->GetComponent<Component::Controller>()->enabled = false;
    mLeftSpawnNode->GetComponent<Component::Controller>()->enabled = false;
    mRightSpawnNode->GetComponent<Component::Controller>()->enabled = false;
    mNode->GetComponent<Component::ParticleEmitter>()->enabled = true;
    mNode->GetComponent<Component::Physics>()->acceleration = glm::vec3(0, 0, 0);
    mNode->GetComponent<Component::Health>()->regainAmount = 0.f;
}

void Player1::AddEnginePartilces(Entity* entity) {
    entity->GetComponent<Component::RelativeTransform>()->Move(0.f, -1.2f, 0.f);
    Component::ParticleEmitter* emitter = entity->AddComponent<Component::ParticleEmitter>();
    emitter->emitterType = Component::ParticleEmitter::POINT;
    emitter->maxEmitTime = emitter->minEmitTime = 0.02 / 20.f;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 0.0;
    emitter->particleType.textureIndex = Component::ParticleEmitter::BLUE;
    emitter->particleType.minLifetime = emitter->particleType.maxLifetime = .08f;
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

void Player1::CreateBarrel(Barrel* barrel) {
    barrel->node->AddComponent<Component::Animation>();

    barrel->barrel[0] = CreateEntity();
    barrel->barrel[0]->AddComponent<Component::RelativeTransform>()->parentEntity = barrel->node;
    barrel->barrel[0]->GetComponent<Component::Transform>()->Move(1.f, 0.f, 0.f);
    barrel->barrel[0]->AddComponent<Component::Mesh>()->geometry = mTurretBarrelModel;
    barrel->barrel[0]->AddComponent<Component::Material>()->SetDiffuse("Resources/turret/turret_diff.png");
    barrel->barrel[0]->AddComponent<Component::Animation>();

    barrel->barrel[1] = CreateEntity();
    barrel->barrel[1]->AddComponent<Component::RelativeTransform>()->parentEntity = barrel->node;
    barrel->barrel[1]->GetComponent<Component::Transform>()->Move(-1.f, 0.f, 0.f);
    barrel->barrel[1]->AddComponent<Component::Mesh>()->geometry = mTurretBarrelModel;
    barrel->barrel[1]->AddComponent<Component::Material>()->SetDiffuse("Resources/turret/turret_diff.png");
    barrel->barrel[1]->AddComponent<Component::Animation>();
}

void Player1::mUpdateFunction() {
    // Update health texture
    if (GetHealth() >= 2.f*(mNode->GetComponent<Component::Health>()->maxHealth / 3.f)) {
        mState = LIGHTDAMAGE;
        mLight->GetComponent<Component::SpotLight>()->color = glm::vec3(0.f, 1.f, 0.f);
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
        mFrontEngineLeft->GetComponent<Component::ParticleEmitter>()->enabled = false;
        mFrontEngineRight->GetComponent<Component::ParticleEmitter>()->enabled = true;
        mBackEngineLeft->GetComponent<Component::ParticleEmitter>()->enabled = true;
        mBackEngineRight->GetComponent<Component::ParticleEmitter>()->enabled = false;
    } else if (GetHealth() >= 0.01f)  {
        mState = HEAVYDAMAGE;
        mLight->GetComponent<Component::SpotLight>()->color = glm::vec3(1.f, 0.0f, 0.0f);
        mBottomLight->GetComponent<Component::PointLight>()->color = glm::vec3(1.f, 0.32f, 0.f);
        mBody->GetComponent<Component::Material>()->diffuse = mHeavyDamageTexture;
        mFrontEngineLeft->GetComponent<Component::ParticleEmitter>()->enabled = false;
        mFrontEngineRight->GetComponent<Component::ParticleEmitter>()->enabled = true;
        mBackEngineLeft->GetComponent<Component::ParticleEmitter>()->enabled = true;
        mBackEngineRight->GetComponent<Component::ParticleEmitter>()->enabled = true;
    } else {
        mState = DEAD;
        
        mLight->GetComponent<Component::SpotLight>()->color = glm::vec3(1.f, 0.0f, 0.0f);
        mBottomLight->GetComponent<Component::PointLight>()->color = glm::vec3(1.f, 0.f, 0.f);
        mBody->GetComponent<Component::Material>()->diffuse = mDeadTexture;
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
    float particlesFactor = 20.f + velocityFactor * 2.f;
    Component::ParticleEmitter* emitter = mBackEngineRightParticles->GetComponent<Component::ParticleEmitter>();
    emitter->particleType.minVelocity = emitter->particleType.maxVelocity = particlesFactor * glm::vec3(0.f, -1.f, 0.f);
    emitter = mBackEngineLeftParticles->GetComponent<Component::ParticleEmitter>();
    emitter->particleType.minVelocity = emitter->particleType.maxVelocity = particlesFactor * glm::vec3(0.f, -1.f, 0.f);
    emitter = mFrontEngineRightParticles->GetComponent<Component::ParticleEmitter>();
    emitter->particleType.minVelocity = emitter->particleType.maxVelocity = particlesFactor * glm::vec3(0.f, -1.f, 0.f);
    emitter = mFrontEngineLeftParticles->GetComponent<Component::ParticleEmitter>();
    emitter->particleType.minVelocity = emitter->particleType.maxVelocity = particlesFactor * glm::vec3(0.f, -1.f, 0.f);

    // Update body
    mBody->GetComponent<Component::Transform>()->pitch = pitchFactor * 10.f * velocityFactor;
    mBody->GetComponent<Component::Transform>()->roll = rollFactor * 15.f * velocityFactor;

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

void Player1::SetYaw(float yaw) {
    mNode->GetComponent<Component::Transform>()->yaw = yaw;
}

void Player1::SetJoystickAim(bool joystickAim) {
    std::vector<void(*)(Component::Controller* controller, float deltaTime)>& vec = mNode->GetComponent<Component::Controller>()->controlSchemes;
    vec.erase(std::remove(vec.begin(), vec.end(), ControlScheme::MouseRotate), vec.end());
    vec.erase(std::remove(vec.begin(), vec.end(), ControlScheme::Aim), vec.end());
    if (joystickAim)
        vec.push_back(&ControlScheme::Aim);
    else
        vec.push_back(&ControlScheme::MouseRotate);

}

void Player1::SetPlayerID(InputHandler::Player id) {
    mNode->GetComponent<Component::Controller>()->playerID = id;
    mLeftSpawnNode->GetComponent<Component::Controller>()->playerID = id;
    mRightSpawnNode->GetComponent<Component::Controller>()->playerID = id;
}

void Player1::SetDevice(InputHandler::Device device) {
    mNode->GetComponent<Component::Controller>()->device = device;
}
