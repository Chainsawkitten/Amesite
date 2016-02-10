#include "Material.hpp"

using namespace Component;

Material::Material(Entity* entity) : SuperComponent(entity) {
    diffuse = nullptr;
    specular = nullptr;
}
