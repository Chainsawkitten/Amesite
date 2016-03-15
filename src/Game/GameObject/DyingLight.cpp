#include "DyingLight.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>

#include <Component/Transform.hpp>
#include <Component/ParticleEmitter.hpp>
#include <Component/PointLight.hpp>
#include "../Component/LifeTime.hpp"
#include "../Component/Update.hpp"

using namespace GameObject;

DyingLight::DyingLight(Scene* scene) : SuperGameObject(scene) {
    node = CreateEntity();
    Component::PointLight* light = node->AddComponent<Component::PointLight>();
    light->color = glm::vec3(1, 0, 0);
    light->ambientCoefficient;
    node->AddComponent<Component::Transform>();
    node->AddComponent<Component::LifeTime>()->lifeTime = 0.5f;

    node->AddComponent<Component::Update>()->updateFunction = std::bind(&DyingLight::updateFunction, this);

}

DyingLight::~DyingLight() {
}

void DyingLight::updateFunction() {

    node->GetComponent<Component::PointLight>()->attenuation *= 1.10f;

}
