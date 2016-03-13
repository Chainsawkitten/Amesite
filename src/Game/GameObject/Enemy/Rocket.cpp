#include "Rocket.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include "../../Component/Controller.hpp"
#include "../../Component/Health.hpp"
#include "../../Component/Spawner.hpp"
#include "../../Component/Explode.hpp"
#include "../../Component/Update.hpp"
#include "../../Component/GridCollide.hpp"
#include "../../Component/Damage.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/RelativeTransform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/Collider2DCircle.hpp>
#include <Engine/Component/Animation.hpp>
#include <Engine/Component/ParticleEmitter.hpp>
#include <Engine/Component/PointLight.hpp>

#include "../../Util/ControlSchemes.hpp"

using namespace GameObject;

Rocket::Rocket(Scene* scene) : SuperEnemy(scene) {
    mActiveGlow = Resources().CreateTexture2DFromFile("Resources/enemy_glow.png");

    node->AddComponent<Component::Transform>()->scale *= 0.15f;
    node->AddComponent<Component::Collider2DCircle>()->radius = 9.0f;
    node->AddComponent<Component::Physics>()->velocityDragFactor = 0.f;
    node->GetComponent<Component::Physics>()->maxVelocity *= 1.15f;
    node->AddComponent<Component::Health>()->faction = 1;
    node->GetComponent<Component::Health>()->health = 300.f;
    node->GetComponent<Component::Health>()->removeOnLowHealth = false;
    node->AddComponent<Component::Explode>()->lifeTime = 0.25f;
    node->GetComponent<Component::Explode>()->size = 20.f;
    node->GetComponent<Component::Explode>()->particleTextureIndex = Component::ParticleEmitter::PURPLE;
    node->GetComponent<Component::Explode>()->sound = true;
    node->AddComponent<Component::Update>()->updateFunction = std::bind(&Rocket::mUpdateFunction, this);
    node->AddComponent<Component::Controller>()->controlSchemes.push_back(ControlScheme::LookAtClosestPlayer);
    node->GetComponent<Component::Controller>()->controlSchemes.push_back(ControlScheme::AccelerateTowardsClosestPlayer);
    node->AddComponent<Component::GridCollide>()->removeOnImpact = false;
    node->AddComponent<Component::Damage>()->faction = 1;
    node->GetComponent<Component::Damage>()->removeOnImpact = false;
    node->AddComponent<Component::PointLight>()->color = glm::vec3(0.67f, 0.f, 0.72f);

    body = CreateEntity();
    body->AddComponent<Component::RelativeTransform>()->Move(0, 0, 5.5f);
    body->GetComponent<Component::RelativeTransform>()->parentEntity = node;
    body->AddComponent<Component::Mesh>()->geometry = mBodyModel = Resources().CreateOBJModel("Resources/rocket_body.obj");
    body->AddComponent<Component::Material>();
    body->GetComponent<Component::Material>()->SetDiffuse("Resources/enemy_diff.png");
    body->GetComponent<Component::Material>()->SetSpecular("Resources/enemy_spec.png");
    mDeactiveGlow = body->GetComponent<Component::Material>()->glow;
    Component::Animation::AnimationClip* idleHead = body->AddComponent<Component::Animation>()->CreateAnimationClip("idle");
    idleHead->CreateKeyFrame(glm::vec3(-0.15f, 0.f, 0.f), 0.f, 0.f, 0, 3.f, false, true);
    idleHead->CreateKeyFrame(glm::vec3(0.15f, 0.f, 0.f), 0.f, 0.f, 0.f, 3.f, false, true);
    body->GetComponent<Component::Animation>()->Start("idle");

    Component::ParticleEmitter* emitter = body->AddComponent<Component::ParticleEmitter>();
    emitter->emitterType = Component::ParticleEmitter::POINT;
    emitter->maxEmitTime = 0.02;
    emitter->minEmitTime = 0.016;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 0.0;
    emitter->particleType.textureIndex = Component::ParticleEmitter::PURPLE;
    emitter->particleType.minLifetime = .01f * 40.f;
    emitter->particleType.maxLifetime = .02f * 40.f;
    emitter->particleType.minVelocity = glm::vec3(-.3f, 0.f, -.2f);
    emitter->particleType.maxVelocity = glm::vec3(.3f, 0.f, .2f);
    emitter->particleType.minSize = glm::vec2(.5f, .5f) * 8.f;
    emitter->particleType.maxSize = glm::vec2(.7f, .7f) * 8.f;
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(.8f, .8f, .8f);
    emitter->particleType.startAlpha = 1.f;
    emitter->particleType.midAlpha = 1.f;
    emitter->particleType.endAlpha = 0.f;

    Deactivate();
}

Rocket::~Rocket() {
    Resources().FreeOBJModel(mBodyModel);
    if (Active())
        Resources().FreeTexture2D(mDeactiveGlow);
    else
        Resources().FreeTexture2D(mActiveGlow);
}

float Rocket::GetHealth() {
    if (node->GetComponent<Component::GridCollide>()->hasCollided == true) {
        return 0.f;
    }
    return node->GetComponent<Component::Health>()->health;
}

void Rocket::Activate() {
    SuperEnemy::Activate();
    node->GetComponent<Component::Controller>()->enabled = true;
    body->GetComponent<Component::Material>()->glow = mActiveGlow;
    node->GetComponent<Component::PointLight>()->intensity = 10.f;
}

void Rocket::Deactivate() {
    SuperEnemy::Deactivate();
    node->GetComponent<Component::Controller>()->enabled = false;
    body->GetComponent<Component::Material>()->glow = mDeactiveGlow;
    node->GetComponent<Component::Physics>()->acceleration = glm::vec3(0.f, 0.f, 0.f);
    node->GetComponent<Component::PointLight>()->intensity = 0.f;
}

void Rocket::mUpdateFunction() {
    SuperEnemy::mUpdateFunction();
    Component::Physics* physics = node->GetComponent<Component::Physics>();
    float velocityFactor = glm::length(physics->velocity) / glm::length(physics->maxVelocity);
    physics->angularVelocity.y = velocityFactor;
    node->GetComponent<Component::Transform>()->pitch = -glm::min((1.f * velocityFactor) * 90.f, 90.f);
}
