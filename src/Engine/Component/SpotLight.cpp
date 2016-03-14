#include "SpotLight.hpp"
#include "../Entity/Entity.hpp"

using namespace Component;

SpotLight::SpotLight(Entity* entity) : SuperComponent(entity) {
    color = glm::vec3(1.f, 1.f, 1.f);
    ambientCoefficient = 0.f;
    attenuation = 1.f;
    intensity = 1.f;
    coneAngle = 15.f;
}
