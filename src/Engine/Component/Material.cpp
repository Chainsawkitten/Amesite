#include "Material.hpp"

#include "../Resources.hpp"
#include "../Texture/Texture2D.hpp"

using namespace Component;

Material::Material(Entity* entity) : SuperComponent(entity) {
    diffuse = Resources().CreateTexture2DFromFile("Resources/DefaultDiffuse.png", true);
    specular = Resources().CreateTexture2DFromFile("Resources/DefaultSpecular.png");
}

Material::~Material() {
    Resources().FreeTexture2DFromFile(diffuse);
    Resources().FreeTexture2DFromFile(specular);
}
