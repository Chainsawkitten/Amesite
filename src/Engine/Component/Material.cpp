#include "Material.hpp"

#include "../Resources.hpp"
#include "../Texture/Texture2D.hpp"

using namespace Component;

Material::Material(Entity* entity) : SuperComponent(entity) {
    diffuse = Resources().CreateTexture2DFromFile("Resources/default/DefaultDiffuse.png", true);
    normal = Resources().CreateTexture2DFromFile("Resources/default/DefaultNormal.png");
    specular = Resources().CreateTexture2DFromFile("Resources/default/DefaultSpecular.png");
    glow = Resources().CreateTexture2DFromFile("Resources/default/DefaultGlow.png");
}

Material::~Material() {
    Resources().FreeTexture2D(diffuse);
    Resources().FreeTexture2D(normal);
    Resources().FreeTexture2D(specular);
    Resources().FreeTexture2D(glow);
}

void Material::SetDiffuse(const char* filename) {
    if (diffuse != nullptr)
        Resources().FreeTexture2D(diffuse);
    
    diffuse = Resources().CreateTexture2DFromFile(filename, true);
}

void Material::SetNormal(const char* filename) {
    if (normal != nullptr)
        Resources().FreeTexture2D(normal);
    
    normal = Resources().CreateTexture2DFromFile(filename, true);
}

void Material::SetSpecular(const char* filename) {
    if (specular != nullptr)
        Resources().FreeTexture2D(specular);
    
    specular = Resources().CreateTexture2DFromFile(filename);
}

void Material::SetGlow(const char* filename) {
    if (glow != nullptr)
        Resources().FreeTexture2D(glow);
    
    glow = Resources().CreateTexture2DFromFile(filename);
}
