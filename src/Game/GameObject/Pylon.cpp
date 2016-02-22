#include "Pylon.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include "../Component/Health.hpp"
#include "../Component/Spawner.hpp"
#include "../Component/Explode.hpp"
#include "../Component/Controller.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/RelativeTransform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/Collider2DCircle.hpp>
#include <Engine/Component/Animation.hpp>
#include <Engine/Component/ParticleEmitter.hpp>

#include "../Util/ControlSchemes.hpp"

using namespace GameObject;

Pylon::Pylon(Scene* scene) : SuperGameObject(scene) {
    node = CreateEntity(scene);
    node->AddComponent<Component::Transform>()->scale *= 0.2f;
    node->AddComponent<Component::Collider2DCircle>()->radius = 9.f;
    node->AddComponent<Component::Health>()->faction = 1;
    node->GetComponent<Component::Health>()->health = 50.f;
    node->AddComponent<Component::Explode>()->lifeTime = 0.25f;
    node->GetComponent<Component::Explode>()->size = 8.f;
    node->GetComponent<Component::Explode>()->particleTextureIndex = Component::ParticleEmitter::FIRE;

    body = CreateEntity(scene);
    body->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    body->AddComponent<Component::Mesh>()->geometry = mBody = Resources().CreateOBJModel("Resources/crystal/Crystal_01.obj");
    body->AddComponent<Component::Material>();
    body->GetComponent<Component::Material>()->SetDiffuse("Resources/crystal/Crystal_01_diff.png");
    body->GetComponent<Component::Material>()->SetSpecular("Resources/crystal/Crystal_01_spec.png");
    body->GetComponent<Component::Material>()->SetGlow("Resources/crystal/Crystal_01_glow.png");
    Component::Animation::AnimationClip* idleBody = body->AddComponent<Component::Animation>()->CreateAnimationClip("idle");
    idleBody->CreateKeyFrame(glm::vec3(0.3f, 0.f, 0.f), 0.f, 0.f, 0, 1.5f, false, true);
    idleBody->CreateKeyFrame(glm::vec3(0.f, 0.3f, 0.3f), 0.f, 0.f, 0.f, 1.5f, false, true);
    idleBody->CreateKeyFrame(glm::vec3(0.3f, 0.3f, -0.3f), 0.f, 0.f, 0.f, 1.5f, false, true);
    idleBody->CreateKeyFrame(glm::vec3(-0.3f, 0.3f, 0.3f), 0.f, 0.f, 0.f, 1.5f, false, true);
    body->GetComponent<Component::Animation>()->Start("idle");

    pylon1 = CreateEntity(scene);
    pylon1->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    pylon1->GetComponent<Component::RelativeTransform>()->scale *= 0.8f;
    pylon1->AddComponent<Component::Mesh>()->geometry = mPylon1 = Resources().CreateOBJModel("Resources/pylon/Pylon_01.obj");
    pylon1->AddComponent<Component::Material>();
    pylon1->GetComponent<Component::Material>()->SetDiffuse("Resources/pylon/Pylon_01_diff.png");
    pylon1->GetComponent<Component::Material>()->SetSpecular("Resources/pylon/Pylon_01_spec.png");
    pylon1->GetComponent<Component::Material>()->SetGlow("Resources/pylon/Pylon_01_glow.png");
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

    pylon2 = CreateEntity(scene);
    pylon2->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    pylon2->GetComponent<Component::RelativeTransform>()->scale *= 0.8f;
    pylon2->AddComponent<Component::Mesh>()->geometry = mPylon2 = Resources().CreateOBJModel("Resources/pylon/Pylon_01.obj");
    pylon2->AddComponent<Component::Material>();
    pylon2->GetComponent<Component::Material>()->SetDiffuse("Resources/pylon/Pylon_01_diff.png");
    pylon2->GetComponent<Component::Material>()->SetSpecular("Resources/pylon/Pylon_01_spec.png");
    pylon2->GetComponent<Component::Material>()->SetGlow("Resources/pylon/Pylon_01_glow.png");
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

    turrent = CreateEntity(scene);
    turrent->AddComponent<Component::RelativeTransform>()->parentEntity = body;
    turrent->GetComponent<Component::RelativeTransform>()->Move(0, 5.f, 0.f);
    turrent->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AlwaysShoot);
    turrent->AddComponent<Component::Spawner>()->delay = 0.5f;
    turrent->GetComponent<Component::Spawner>()->faction = 1;
}

Pylon::~Pylon() {
    Resources().FreeOBJModel(mBody);
    Resources().FreeOBJModel(mPylon1);
    Resources().FreeOBJModel(mPylon2);
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
