#include "DirectionalLight.hpp"
#include "../Entity/Entity.hpp"

using namespace Component;

DirectionalLight::DirectionalLight(Entity* entity) : SuperComponent(entity) {
    color = glm::vec3(1.f, 1.f, 1.f);
    ambientCoefficient = 0.f;
}
