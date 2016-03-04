#include "PillarBall.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include <Engine/Component/Transform.hpp>
#include <Engine/Component/RelativeTransform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/PointLight.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/ParticleEmitter.hpp>
#include "../Component/Update.hpp"
#include "../Component/Spawner.hpp"
#include "../Component/LifeTime.hpp"

#include "../Util/ControlSchemes.hpp"

using namespace GameObject;

PillarBall::PillarBall(Scene* scene) : SuperGameObject(scene) {
    node = CreateEntity();
    node->AddComponent<Component::Transform>()->scale *= 0.05f;
    node->AddComponent<Component::Physics>()->velocity.y = 20.f;
    node->GetComponent<Component::Physics>()->angularVelocity.y = 0.5f;
    node->GetComponent<Component::Physics>()->angularDragFactor = 0.f;
    node->GetComponent<Component::Physics>()->velocityDragFactor = 0.f;
    node->AddComponent<Component::LifeTime>()->lifeTime = 10.f;
    node->AddComponent<Component::Update>()->updateFunction = std::bind(&PillarBall::mUpdateFunction, this);

    mBallModel = Resources().CreateOBJModel("Resources/portal.obj");

    ball1 = CreateEntity();
    ball1->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    ball1->GetComponent<Component::RelativeTransform>()->position = glm::vec3(30.f, 0.f, 0.f);
    ball1->AddComponent<Component::Material>();
    ball1->AddComponent<Component::Mesh>()->geometry = mBallModel;
    AddPartilces(ball1);

    ball2 = CreateEntity();
    ball2->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    ball2->GetComponent<Component::RelativeTransform>()->position = glm::vec3(-30.f, 0.f, 0.f);
    ball2->AddComponent<Component::Material>();
    ball2->AddComponent<Component::Mesh>()->geometry = mBallModel;
    AddPartilces(ball2);
}

PillarBall::~PillarBall() {
    Resources().FreeOBJModel(mBallModel);
    //node->RemoveComponent<Component::Update>();
}

void PillarBall::mUpdateFunction() {
    //float speedFactor = (10.f - node->GetComponent<Component::LifeTime>()->lifeTime);
    //node->GetComponent<Component::Physics>()->velocity.y = 5.f + speedFactor * 20.f;
    //node->GetComponent<Component::Physics>()->angularAcceleration.y = 0.1f + 0.1f * speedFactor;
}

void PillarBall::AddPartilces(Entity* entity) {
    entity->GetComponent<Component::RelativeTransform>()->Move(0.f, -1.f, 0.f);
    Component::ParticleEmitter* emitter = entity->AddComponent<Component::ParticleEmitter>();
    emitter->emitterType = Component::ParticleEmitter::POINT;
    emitter->maxEmitTime = 0.02f;
    emitter->minEmitTime = 0.016f;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 0.0;
    emitter->particleType.textureIndex = Component::ParticleEmitter::BLUE;
    emitter->particleType.minLifetime = .04f * 10.f;
    emitter->particleType.maxLifetime = .08f * 10.f;
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
