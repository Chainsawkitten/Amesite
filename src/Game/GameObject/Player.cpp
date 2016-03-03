#include "Player.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include "../Component/Controller.hpp"
#include "../Component/Health.hpp"
#include "../Component/Spawner.hpp"
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

#include "../Util/ControlSchemes.hpp"

using namespace GameObject;

Player::Player(Scene* scene, InputHandler::Player id) : SuperGameObject(scene) {
    mActive = true;
    mRespawnTimer = 5;

    if (id == InputHandler::Player::PLAYER_ONE) {
        mHealthyTexture = Resources().CreateTexture2DFromFile("Resources/ship1_body_diff_healthy.png");
        mMediumDamageTexture = Resources().CreateTexture2DFromFile("Resources/ship1_body_diff_medium_damage.png");
        mHeavyDamageTexture = Resources().CreateTexture2DFromFile("Resources/ship1_body_diff_heavy_damage.png");
    } else {
        mHealthyTexture = Resources().CreateTexture2DFromFile("Resources/ship2_body_diff_healthy.png");
        mMediumDamageTexture = Resources().CreateTexture2DFromFile("Resources/ship2_body_diff_medium_damage.png");
        mHeavyDamageTexture = Resources().CreateTexture2DFromFile("Resources/ship2_body_diff_heavy_damage.png");
        //mHealthyTexture = Resources().CreateTexture2DFromFile("Resources/Ship_Dif.png");
        //mMediumDamageTexture = Resources().CreateTexture2DFromFile("Resources/Ship_Dif.png");
        //mHeavyDamageTexture = Resources().CreateTexture2DFromFile("Resources/Ship_Dif.png");
    }
    
    state = LIGHTDAMAGE;

    node = CreateEntity();
    node->AddComponent<Component::Transform>()->scale *= 0.2f;
    node->AddComponent<Component::Controller>()->speed = 5000.f;
    node->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Move);
    node->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Shield);
    node->AddComponent<Component::Physics>()->velocityDragFactor = 3.f;
    node->AddComponent<Component::Health>()->removeOnLowHealth = false;
    node->GetComponent<Component::Health>()->health = 30.f;
    node->GetComponent<Component::Health>()->maxHealth = 30.f;
    node->GetComponent<Component::Health>()->faction = 0;
    node->AddComponent<Component::Collider2DCircle>()->radius = 10.f;
    node->AddComponent<Component::Animation>();
    Component::Animation::AnimationClip* idleNode = node->GetComponent<Component::Animation>()->CreateAnimationClip("idle");
    idleNode->CreateKeyFrame(glm::vec3(0.1f, 0.f, 0.f), 0.f, 0.f, 0, 1.5f, false, true);
    idleNode->CreateKeyFrame(glm::vec3(0.f, 0.1f, 0.1f), 0.f, 0.f, 0.f, 1.5f, false, true);
    idleNode->CreateKeyFrame(glm::vec3(0.1f, 0.1f, -0.1f), 0.f, 0.f, 0.f, 1.5f, false, true);
    idleNode->CreateKeyFrame(glm::vec3(-0.1f, 0.1f, 0.1f), 0.f, 0.f, 0.f, 1.5f, false, true);
    node->GetComponent<Component::Animation>()->Start("idle");

    body = CreateEntity();
    body->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    body->AddComponent<Component::Mesh>()->geometry = mShipBody = Resources().CreateOBJModel("Resources/ship_body.obj");
    //body->AddComponent<Component::Mesh>()->geometry = mShipBody = Resources().CreateOBJModel("Resources/ship.obj");
    body->AddComponent<Component::Material>();

    Resources().FreeTexture2D(body->GetComponent<Component::Material>()->diffuse);
    body->GetComponent<Component::Material>()->diffuse = mHealthyTexture;
    body->GetComponent<Component::Material>()->SetSpecular("Resources/ship_body_spec.png");
    body->GetComponent<Component::Material>()->SetGlow("Resources/ship_body_glow.png");
    //body->GetComponent<Component::Material>()->SetSpecular("Resources/Ship_Spec.png");
    //body->GetComponent<Component::Material>()->SetNormal("Resources/Ship_NM.png");
    body->AddComponent<Component::Animation>();

    Component::ParticleEmitter* emitter = body->AddComponent<Component::ParticleEmitter>();
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

    light = CreateEntity();
    light->AddComponent<Component::RelativeTransform>()->Move(0, 1, 0);
    light->GetComponent<Component::RelativeTransform>()->parentEntity = body;
    light->AddComponent<Component::Animation>();
    light->AddComponent<Component::SpotLight>()->coneAngle = 45.f;
    light->GetComponent<Component::SpotLight>()->attenuation = 0.05f;

    leftTurret = CreateEntity();
    leftTurret->AddComponent<Component::RelativeTransform>()->Move(2.5f, 0, 13);
    leftTurret->GetComponent<Component::RelativeTransform>()->parentEntity = body;
    leftTurret->AddComponent<Component::Animation>();
    leftTurret->AddComponent<Component::Spawner>()->delay = 0.25f;
    leftTurret->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AimedFire);
    Component::SoundSource* sound = leftTurret->AddComponent<Component::SoundSource>();
    mShootSound = Resources().CreateSound("Resources/Laser.ogg");
    sound->soundBuffer = mShootSound;
    sound->gain = 2.f;

    rightTurret = CreateEntity();
    rightTurret->AddComponent<Component::RelativeTransform>()->Move(-2.5f, 0, 13);
    rightTurret->GetComponent<Component::RelativeTransform>()->parentEntity = body;
    rightTurret->AddComponent<Component::Animation>();
    rightTurret->AddComponent<Component::Spawner>()->delay = 0.25f;
    rightTurret->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AimedFire);

    frontEngineLeft = CreateEntity();
    frontEngineLeft->AddComponent<Component::RelativeTransform>()->Move(8.5f, 0.f, 8.3f);
    frontEngineLeft->GetComponent<Component::RelativeTransform>()->parentEntity = body;
    frontEngineLeft->GetComponent<Component::RelativeTransform>()->scale *= 1.1f;
    frontEngineLeft->AddComponent<Component::Animation>();
    frontEngineLeft->AddComponent<Component::Mesh>()->geometry = mShipFrontEngineLeft = Resources().CreateOBJModel("Resources/ship_frontEngine.obj");
    frontEngineLeft->AddComponent<Component::Material>();
    frontEngineLeft->GetComponent<Component::Material>()->SetDiffuse("Resources/ship_engine_diff.png");
    Entity* frontEngineLeftParticles = CreateEntity();
    frontEngineLeftParticles->AddComponent<Component::RelativeTransform>()->parentEntity = frontEngineLeft;
    frontEngineLeftParticles->GetComponent<Component::RelativeTransform>()->Move(0.f, -1.f, 0.f);
    AddEnginePartilces(frontEngineLeftParticles);

    frontEngineRight = CreateEntity();
    frontEngineRight->AddComponent<Component::RelativeTransform>()->Move(-8.5f, 0.f, 8.3f);
    frontEngineRight->GetComponent<Component::RelativeTransform>()->parentEntity = body;
    frontEngineRight->GetComponent<Component::RelativeTransform>()->scale *= 1.1f;
    frontEngineRight->GetComponent<Component::RelativeTransform>()->yaw = 180.f;
    frontEngineRight->AddComponent<Component::Animation>();
    frontEngineRight->AddComponent<Component::Mesh>()->geometry = mShipFrontEngineRight = Resources().CreateOBJModel("Resources/ship_frontEngine.obj");
    frontEngineRight->AddComponent<Component::Material>();
    frontEngineRight->GetComponent<Component::Material>()->SetDiffuse("Resources/ship_engine_diff.png");
    Entity* frontEngineRightParticles = CreateEntity();
    frontEngineRightParticles->AddComponent<Component::RelativeTransform>()->parentEntity = frontEngineRight;
    frontEngineRightParticles->GetComponent<Component::RelativeTransform>()->Move(0.f, -1.f, 0.f);
    AddEnginePartilces(frontEngineRightParticles);

    backEngineLeft = CreateEntity();
    backEngineLeft->AddComponent<Component::RelativeTransform>()->Move(10.5f, 0.f, 0.f);
    backEngineLeft->GetComponent<Component::RelativeTransform>()->parentEntity = body;
    backEngineLeft->GetComponent<Component::RelativeTransform>()->scale *= 1.2f;
    backEngineLeft->AddComponent<Component::Animation>();
    backEngineLeft->AddComponent<Component::Mesh>()->geometry = mShipBackEngineLeft = Resources().CreateOBJModel("Resources/ship_backEngine.obj");
    backEngineLeft->AddComponent<Component::Material>();
    backEngineLeft->GetComponent<Component::Material>()->SetDiffuse("Resources/ship_engine_diff.png");
    Entity* backEngineLeftParticles = CreateEntity();
    backEngineLeftParticles->AddComponent<Component::RelativeTransform>()->parentEntity = backEngineLeft;
    backEngineLeftParticles->GetComponent<Component::RelativeTransform>()->Move(0.f, -1.f, 0.f);
    AddEnginePartilces(backEngineLeftParticles);

    backEngineRight = CreateEntity();
    backEngineRight->AddComponent<Component::RelativeTransform>()->Move(-10.5f, 0.f, 0.f);
    backEngineRight->GetComponent<Component::RelativeTransform>()->parentEntity = body;
    backEngineRight->GetComponent<Component::RelativeTransform>()->scale *= 1.2f;
    backEngineRight->GetComponent<Component::RelativeTransform>()->yaw = 180.f;
    backEngineRight->AddComponent<Component::Animation>();
    backEngineRight->AddComponent<Component::Mesh>()->geometry = mShipBackEngineRight = Resources().CreateOBJModel("Resources/ship_backEngine.obj");
    backEngineRight->AddComponent<Component::Material>();
    backEngineRight->GetComponent<Component::Material>()->SetDiffuse("Resources/ship_engine_diff.png");
    Entity* backEngineRightParticles = CreateEntity();
    backEngineRightParticles->AddComponent<Component::RelativeTransform>()->parentEntity = backEngineRight;
    backEngineRightParticles->GetComponent<Component::RelativeTransform>()->Move(0.f, -1.f, 0.f);
    AddEnginePartilces(backEngineRightParticles);


}

Player::~Player() {
    if (state != LIGHTDAMAGE)
        Resources().FreeTexture2D(mHealthyTexture);
    if (state != MEDIUMDAMAGE)
        Resources().FreeTexture2D(mMediumDamageTexture);
    if (state != HEAVYDAMAGE)
        Resources().FreeTexture2D(mHeavyDamageTexture);
    Resources().FreeOBJModel(mShipBody);
    Resources().FreeOBJModel(mShipFrontEngineLeft);
    Resources().FreeOBJModel(mShipFrontEngineRight);
    Resources().FreeOBJModel(mShipBackEngineLeft);
    Resources().FreeOBJModel(mShipBackEngineRight);
    
    Resources().FreeSound(mShootSound);
}

glm::vec3 Player::GetPosition() {
    return node->GetComponent<Component::Transform>()->GetWorldPosition();
}

float Player::GetHealth() {
    return node->GetComponent<Component::Health>()->health;
}

bool Player::Active() {

    return mActive;

}

void Player::Activate() {

    mActive = true;
    node->GetComponent<Component::Controller>()->enabled = true;
    leftTurret->GetComponent<Component::Controller>()->enabled = true;
    rightTurret->GetComponent<Component::Controller>()->enabled = true;
    node->GetComponent<Component::Health>()->health = node->GetComponent<Component::Health>()->maxHealth;

}

void Player::Deactivate() {

    mActive = false;
    node->GetComponent<Component::Controller>()->enabled = false;
    leftTurret->GetComponent<Component::Controller>()->enabled = false;
    rightTurret->GetComponent<Component::Controller>()->enabled = false;

}

void GameObject::Player::UpdatePlayerTexture() {
    if (GetHealth() >= 2.f*(node->GetComponent<Component::Health>()->maxHealth / 3.f)) {
        state = LIGHTDAMAGE;
        body->GetComponent<Component::Material>()->diffuse = mHealthyTexture;
    } else if (GetHealth() >= 1.f*(node->GetComponent<Component::Health>()->maxHealth / 3.f)) {
        state = MEDIUMDAMAGE;
        body->GetComponent<Component::Material>()->diffuse = mMediumDamageTexture;
    } else {
        state = HEAVYDAMAGE;
        body->GetComponent<Component::Material>()->diffuse = mHeavyDamageTexture;
    }
}

void Player::AddEnginePartilces(Entity* entity) {
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
