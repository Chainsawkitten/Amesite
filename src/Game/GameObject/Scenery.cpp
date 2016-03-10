#include "Rock.hpp"

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
#include "../Component/Update.hpp"
#include "../Component/Spawner.hpp"
#include "../Component/LifeTime.hpp"

#include "../Util/GameEntityFactory.hpp"
#include "../Util/ControlSchemes.hpp"

#include "../GameObject/PillarBall.hpp"

using namespace GameObject;

Scenery::Scenery(Scene* scene, char* model, char* diffuse, char* specular, char* glow) : SuperGameObject(scene) {

    node = CreateEntity();
    node->AddComponent<Component::Transform>();
    node->AddComponent<Component::Material>();
    node->AddComponent<Component::Mesh>()->geometry = mModel = Resources().CreateOBJModel(model);
    if(diffuse == nullptr)
        node->GetComponent<Component::Material>()->SetDiffuse("Resources/DefaultDiffuse.png");
    else
        node->GetComponent<Component::Material>()->SetDiffuse(diffuse);

    if (specular == nullptr)
        node->GetComponent<Component::Material>()->SetSpecular("Resources/DefaultSpecular.png");
    else
        node->GetComponent<Component::Material>()->SetSpecular(specular);

    if (glow == nullptr)
        node->GetComponent<Component::Material>()->SetGlow("Resources/DefaultGlow.png");
    else
        node->GetComponent<Component::Material>()->SetGlow(glow);

    //mLight = CreateEntity();
    //mLight->AddComponent<Component::PointLight>();
    //mLight->GetComponent<Component::PointLight>()->attenuation = 5.f;
    //mLight->AddComponent<Component::RelativeTransform>()->parentEntity = node;

}

Scenery::~Scenery() {
    Resources().FreeOBJModel(mModel);
}