#include "Pylon.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>

#include <Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include "../../Component/Health.hpp"
#include "../../Component/Spawner.hpp"
#include "../../Component/Explode.hpp"
#include "../../Component/Controller.hpp"
#include "../../Component/Update.hpp"
#include "../../Component/GridCollide.hpp"
#include <Component/Physics.hpp>
#include <Component/Transform.hpp>
#include <Component/RelativeTransform.hpp>
#include <Component/Mesh.hpp>
#include <Component/Material.hpp>
#include <Component/Collider2DCircle.hpp>
#include <Component/Animation.hpp>
#include <Component/ParticleEmitter.hpp>
#include <Component/PointLight.hpp>


#include "../../Util/ControlSchemes.hpp"

using namespace GameObject;

Pylon::Pylon(Scene* scene) : SuperEnemy(scene) {
    mActiveGlowBody = Resources().CreateTexture2DFromFile("Resources/enemy_glow.png");
    mActiveGlowPylon1 = Resources().CreateTexture2DFromFile("Resources/enemy_glow.png");
    mActiveGlowPylon2 = Resources().CreateTexture2DFromFile("Resources/enemy_glow.png");

    node->AddComponent<Component::Transform>()->scale *= 0.2f;
    node->AddComponent<Component::Collider2DCircle>()->radius = 9.f;
    node->AddComponent<Component::Health>()->faction = 1;
    node->GetComponent<Component::Health>()->health = 50.f;
    node->GetComponent<Component::Health>()->removeOnLowHealth = false;
    node->AddComponent<Component::Explode>()->lifeTime = 0.25f;
    node->GetComponent<Component::Explode>()->size = 8.f;
    node->GetComponent<Component::Explode>()->particleTextureIndex = Component::ParticleEmitter::PURPLE;
    node->GetComponent<Component::Explode>()->sound = true;
    node->GetComponent<Component::Explode>()->type = Component::Explode::ENEMY;
    node->AddComponent<Component::Update>()->updateFunction = std::bind(&Pylon::mUpdateFunction, this);
    node->AddComponent<Component::Physics>()->maxVelocity *= 0.95f;
    node->AddComponent<Component::GridCollide>();
    node->GetComponent<Component::GridCollide>()->removeOnImpact = false;
    node->AddComponent<Component::Controller>();
    node->GetComponent<Component::Controller>()->controlSchemes.push_back(ControlScheme::AccelerateTowardsClosestPlayer);
    node->AddComponent<Component::PointLight>()->color = glm::vec3(0.67f, 0.f, 0.72f);

    body = CreateEntity();
    body->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    body->AddComponent<Component::Mesh>()->geometry = mBodyModel = Resources().CreateOBJModel("Resources/pylon.obj");
    body->AddComponent<Component::Material>();
    body->GetComponent<Component::Material>()->SetDiffuse("Resources/enemy_diff.png");
    body->GetComponent<Component::Material>()->SetSpecular("Resources/enemy_spec.png");
    mDeactiveGlowBody = body->GetComponent<Component::Material>()->glow;
    Component::Animation::AnimationClip* idleBody = body->AddComponent<Component::Animation>()->CreateAnimationClip("idle");
    idleBody->CreateKeyFrame(glm::vec3(0.3f, 0.f, 0.f), 0.f, 0.f, 0, 1.5f, false, true);
    idleBody->CreateKeyFrame(glm::vec3(0.f, 0.3f, 0.3f), 0.f, 0.f, 0.f, 1.5f, false, true);
    idleBody->CreateKeyFrame(glm::vec3(0.3f, 0.3f, -0.3f), 0.f, 0.f, 0.f, 1.5f, false, true);
    idleBody->CreateKeyFrame(glm::vec3(-0.3f, 0.3f, 0.3f), 0.f, 0.f, 0.f, 1.5f, false, true);
    body->GetComponent<Component::Animation>()->Start("idle");

    pylon1 = CreateEntity();
    pylon1->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    pylon1->GetComponent<Component::RelativeTransform>()->scale *= 0.5f;
    pylon1->AddComponent<Component::Mesh>()->geometry = mPylon1 = Resources().CreateOBJModel("Resources/pylon.obj");
    pylon1->AddComponent<Component::Material>();
    pylon1->GetComponent<Component::Material>()->SetDiffuse("Resources/enemy_diff.png");
    pylon1->GetComponent<Component::Material>()->SetSpecular("Resources/enemy_spec.png");
    mDeactiveGlowPylon1 = pylon1->GetComponent<Component::Material>()->glow;
    Component::Animation::AnimationClip* idlePylon1 = pylon1->AddComponent<Component::Animation>()->CreateAnimationClip("idle");
    idlePylon1->CreateKeyFrame(glm::vec3(0.f, 0.f, 3.f), 0.f, 0.f, 0, 3.f * 0.1f, true, false);
    idlePylon1->CreateKeyFrame(glm::vec3(-3.f, 0.f, 0.f), 0.f, 0.f, 0.f, 3.f * 0.1f, true, false);
    idlePylon1->CreateKeyFrame(glm::vec3(0.f, 0.f, -3.f), 0.f, 0.f, 0, 3.f * 0.1f, true, false);
    idlePylon1->CreateKeyFrame(glm::vec3(3.f, 0.f, 0.f), 0.f, 0.f, 0.f, 3.f * 0.1f, true, false);
    pylon1->GetComponent<Component::Animation>()->Start("idle");
    pylon1->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AlwaysShoot);
    pylon1->AddComponent<Component::Spawner>()->delay = 0.5f;
    pylon1->GetComponent<Component::Spawner>()->faction = 1;
    AddPylonPartilces(pylon1);

    pylon2 = CreateEntity();
    pylon2->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    pylon2->GetComponent<Component::RelativeTransform>()->scale *= 0.5f;
    pylon2->AddComponent<Component::Mesh>()->geometry = mPylon2 = Resources().CreateOBJModel("Resources/pylon.obj");
    pylon2->AddComponent<Component::Material>();
    pylon2->GetComponent<Component::Material>()->SetDiffuse("Resources/enemy_diff.png");
    pylon2->GetComponent<Component::Material>()->SetSpecular("Resources/enemy_spec.png");
    mDeactiveGlowPylon2 = pylon2->GetComponent<Component::Material>()->glow;
    Component::Animation::AnimationClip* idlePylon2 = pylon2->AddComponent<Component::Animation>()->CreateAnimationClip("idle");
    idlePylon2->CreateKeyFrame(glm::vec3(0.f, 0.f, -3.f), 0.f, 0.f, 0, 3.f * 0.1f, true, false);
    idlePylon2->CreateKeyFrame(glm::vec3(3.f, 0.f, 0.f), 0.f, 0.f, 0.f, 3.f * 0.1f, true, false);
    idlePylon2->CreateKeyFrame(glm::vec3(0.f, 0.f, 3.f), 0.f, 0.f, 0, 3.f * 0.1f, true, false);
    idlePylon2->CreateKeyFrame(glm::vec3(-3.f, 0.f, 0.f), 0.f, 0.f, 0.f, 3.f * 0.1f, true, false);
    pylon2->GetComponent<Component::Animation>()->Start("idle");
    pylon2->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AlwaysShoot);
    pylon2->AddComponent<Component::Spawner>()->delay = 0.5f;
    pylon2->GetComponent<Component::Spawner>()->faction = 1;
    AddPylonPartilces(pylon2);

    turret = CreateEntity();
    turret->AddComponent<Component::RelativeTransform>()->parentEntity = body;
    turret->GetComponent<Component::RelativeTransform>()->Move(0, 5.f, 0.f);
    turret->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AlwaysShootClosestPlayer);
    turret->AddComponent<Component::Spawner>()->delay = 0.5f;
    turret->GetComponent<Component::Spawner>()->faction = 1;

    Deactivate();
}

Pylon::~Pylon() {
    Resources().FreeOBJModel(mBodyModel);
    Resources().FreeOBJModel(mPylon1);
    Resources().FreeOBJModel(mPylon2);
    if (Active()) {
        Resources().FreeTexture2D(mDeactiveGlowBody);
        Resources().FreeTexture2D(mDeactiveGlowPylon1);
        Resources().FreeTexture2D(mDeactiveGlowPylon2);
    } else {
        Resources().FreeTexture2D(mActiveGlowBody);
        Resources().FreeTexture2D(mActiveGlowPylon1);
        Resources().FreeTexture2D(mActiveGlowPylon2);
    }
}

void Pylon::AddPylonPartilces(Entity* entity) {
    Component::ParticleEmitter* emitter = entity->AddComponent<Component::ParticleEmitter>();
    emitter->emitterType = Component::ParticleEmitter::POINT;
    emitter->maxEmitTime = 0.02;
    emitter->minEmitTime = 0.016;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 0.0;
    emitter->particleType.textureIndex = Component::ParticleEmitter::PURPLE;
    emitter->particleType.minLifetime = .01f * 30.f;
    emitter->particleType.maxLifetime = .02f * 30.f;
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

float Pylon::GetHealth() {
    return node->GetComponent<Component::Health>()->health;
}

void Pylon::Activate() {
    SuperEnemy::Activate();
    node->GetComponent<Component::Controller>()->enabled = true;
    turret->GetComponent<Component::Controller>()->enabled = true;
    pylon1->GetComponent<Component::Controller>()->enabled = true;
    pylon2->GetComponent<Component::Controller>()->enabled = true;
    body->GetComponent<Component::Material>()->glow = mActiveGlowBody;
    pylon1->GetComponent<Component::Material>()->glow = mActiveGlowPylon1;
    pylon2->GetComponent<Component::Material>()->glow = mActiveGlowPylon2;
    pylon1->GetComponent<Component::ParticleEmitter>()->enabled = true;
    pylon2->GetComponent<Component::ParticleEmitter>()->enabled = true;
    node->GetComponent<Component::PointLight>()->intensity = 10.f;
}

void Pylon::Deactivate() {
    SuperEnemy::Deactivate();
    node->GetComponent<Component::Controller>()->enabled = false;
    node->GetComponent<Component::Physics>()->velocity = glm::vec3(0.f, 0.f, 0.f);
    turret->GetComponent<Component::Controller>()->enabled = false;
    pylon1->GetComponent<Component::Controller>()->enabled = false;
    pylon2->GetComponent<Component::Controller>()->enabled = false;
    body->GetComponent<Component::Material>()->glow = mDeactiveGlowBody;
    pylon1->GetComponent<Component::Material>()->glow = mDeactiveGlowPylon1;
    pylon2->GetComponent<Component::Material>()->glow = mDeactiveGlowPylon2;
    pylon1->GetComponent<Component::ParticleEmitter>()->enabled = false;
    pylon2->GetComponent<Component::ParticleEmitter>()->enabled = false;
    node->GetComponent<Component::PointLight>()->intensity = 0.f;
}

void Pylon::mUpdateFunction() {
    SuperEnemy::mUpdateFunction();
}
