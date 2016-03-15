#include "PillarBall.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>

#include <Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include <Component/Transform.hpp>
#include <Component/RelativeTransform.hpp>
#include <Component/Mesh.hpp>
#include <Component/Material.hpp>
#include <Component/PointLight.hpp>
#include <Component/Physics.hpp>
#include <Component/ParticleEmitter.hpp>
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

    //mBallModel = Resources().CreateOBJModel("Resources/portal.obj");

    ball1 = CreateEntity();
    ball1->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    ball1->GetComponent<Component::RelativeTransform>()->position = glm::vec3(30.f, 0.f, 0.f);
    //ball1->AddComponent<Component::Material>();
    //ball1->AddComponent<Component::Mesh>()->geometry = mBallModel;
    AddPartilces(ball1);

    ball2 = CreateEntity();
    ball2->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    ball2->GetComponent<Component::RelativeTransform>()->position = glm::vec3(-30.f, 0.f, 0.f);
    //ball2->AddComponent<Component::Material>();
    //ball2->AddComponent<Component::Mesh>()->geometry = mBallModel;
    AddPartilces(ball2);

    startPosition = glm::vec2(0.f, 0.f);
    destination = glm::vec2(1.f, 0.f);
    maxHeight = 10.f;
}

PillarBall::~PillarBall() {
   // Resources().FreeOBJModel(mBallModel);
    //node->RemoveComponent<Component::Update>();
}

void PillarBall::mUpdateFunction() {
    Component::Transform* transform = node->GetComponent<Component::Transform>();
    Component::Physics* physcis = node->GetComponent<Component::Physics>();
    float maxheight = startHeight + maxHeight;
    float heightFactor = glm::distance(glm::vec2(transform->position.x, transform->position.z), startPosition) / glm::distance(destination, startPosition); //[0,1]
    if (heightFactor > 0.5f)
        heightFactor = 1.f - heightFactor;
    float y = maxheight * glm::sqrt(2 * heightFactor) + startHeight;

    Component::LifeTime* lifeTimeComp = node->GetComponent<Component::LifeTime>();
    float lifeTimeFactor = (1.f - lifeTimeComp->lifeTime / lifeTimeComp->initialLifeTime); //[0,1]
    glm::vec2 position = glm::vec2(destination - startPosition) * lifeTimeFactor;
    glm::vec3 targetPosition = glm::vec3(startPosition.x + position.x, y, startPosition.y + position.y);
    
    glm::vec3 velocity = targetPosition - transform->position;
    physcis->velocity = velocity;
    physcis->maxVelocity = glm::length(velocity);

    //transform->position = targetPosition;
    //transform->UpdateModelMatrix();
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
    emitter->particleType.minVelocity = glm::vec3(0.f, 0.f, 0.f);
    emitter->particleType.maxVelocity = glm::vec3(0.f, 0.f, 0.f);
    emitter->particleType.minSize = glm::vec2(.5f, .5f) * 2.f;
    emitter->particleType.maxSize = glm::vec2(.7f, .7f) * 2.f;
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(.8f, .8f, .8f);
    emitter->particleType.startAlpha = 1.f;
    emitter->particleType.midAlpha = 1.f;
    emitter->particleType.endAlpha = 0.f;
}
