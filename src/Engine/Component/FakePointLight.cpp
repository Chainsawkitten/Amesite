#include "FakePointLight.hpp"
#include "../Entity/Entity.hpp"

using namespace Component;

FakePointLight::FakePointLight(Entity* entity) : SuperComponent(entity) {
    color = glm::vec3(1.f, 1.f, 1.f);
    attenuation = 1.f;
    intensity = 1.f;
}
