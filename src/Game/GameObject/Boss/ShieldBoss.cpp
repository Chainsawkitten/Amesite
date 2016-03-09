#include "ShieldBoss.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include "../../Component/Health.hpp"
#include "../../Component/Explode.hpp"
#include "../../Component/Update.hpp"
#include "../../Component/Spawner.hpp"
#include "../../Component/Reflect.hpp"
#include "../../Component/Controller.hpp"
#include "../../Component/Damage.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/RelativeTransform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/Collider2DCircle.hpp>
#include <Engine/Component/Animation.hpp>
#include <Engine/Component/ParticleEmitter.hpp>
#include <Engine/Component/Physics.hpp>

#include "../../Util/ControlSchemes.hpp"

using namespace GameObject;

ShieldBoss::ShieldBoss(Scene* scene) : SuperBoss(scene) {

    moveCoverIn = false;

    node->AddComponent<Component::Transform>()->scale *= 0.7f;
    node->AddComponent<Component::Update>()->updateFunction = std::bind(&ShieldBoss::mUpdateFunction, this);
    node->AddComponent<Component::Spawner>()->delay = 0.8f;

    body->GetComponent<Component::Transform>()->scale *= 1.2f;
    body->AddComponent<Component::Mesh>()->geometry = mBodyModel = Resources().CreateOBJModel("Resources/diamond_body.obj");
    body->AddComponent<Component::Material>();
    body->GetComponent<Component::Material>()->SetDiffuse("Resources/pylon_diff.png");
    body->GetComponent<Component::Material>()->SetSpecular("Resources/pylon_spec.png");
    body->GetComponent<Component::Material>()->SetGlow("Resources/pylon_glow.png");
    body->AddComponent<Component::Collider2DCircle>()->radius = 6.f;
    body->AddComponent<Component::Explode>()->size = 100.f;
    body->GetComponent<Component::Explode>()->particleTextureIndex = Component::ParticleEmitter::PURPLE;
    body->GetComponent<Component::Explode>()->lifeTime = 1.5f;
    body->GetComponent<Component::Explode>()->offset.y = 5.0f;
    body->GetComponent<Component::Explode>()->sound = true;
    body->AddComponent<Component::Health>()->faction = 1;
    body->GetComponent<Component::Health>()->removeOnLowHealth = false;
    body->AddComponent<Component::Physics>()->angularDragFactor = 0.f;
    body->GetComponent<Component::Physics>()->angularVelocity.y = -0.1f;
    body->AddComponent<Component::Spawner>()->delay = 0.2;
    body->GetComponent<Component::Spawner>()->faction = 1.f;
    body->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AlwaysShoot);

    mCoverModel = Resources().CreateOBJModel("Resources/ring_segment.obj");
    for (int i = 0; i < 3; i++) {
        coverArr[i].node = CreateEntity();
        coverArr[i].leftShield = CreateEntity();
        coverArr[i].rightShield = CreateEntity();
        CreateCover(&coverArr[i]);
        Component::Transform* transform = coverArr[i].node->GetComponent<Component::Transform>();
        transform->yaw = i * 120.f;
        transform->Move(transform->GetWorldDirection() * 18.f);
    }
}

ShieldBoss::~ShieldBoss() {
    Resources().FreeOBJModel(mBodyModel);
    Resources().FreeOBJModel(mCoverModel);
    //for (int i = 0; i < 3; i++) {
    //    coverArr[i].node->Kill();
    //}
}

float ShieldBoss::GetHealth() {
    return body->GetComponent<Component::Health>()->health;
}

glm::vec3 ShieldBoss::GetPosition() {
    return node->GetComponent<Component::Transform>()->position;
}

void ShieldBoss::CreateCover(Cover* cover) {
    cover->node->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    cover->node->GetComponent<Component::Transform>()->scale *= 2.f;
    cover->node->AddComponent<Component::Mesh>()->geometry = mCoverModel;
    cover->node->AddComponent<Component::Material>();

    cover->node->AddComponent<Component::Reflect>();
    cover->node->AddComponent<Component::Collider2DCircle>()->radius = 4.f;
    cover->node->AddComponent<Component::Damage>()->faction = 1.f;
    cover->node->GetComponent<Component::Damage>()->removeOnImpact = false;

    cover->leftShield->AddComponent<Component::RelativeTransform>()->parentEntity = cover->node;
    cover->leftShield->GetComponent<Component::Transform>()->position.x = 5.f;
    cover->leftShield->AddComponent<Component::Reflect>();
    cover->leftShield->AddComponent<Component::Collider2DCircle>()->radius = 4.f;
    cover->leftShield->AddComponent<Component::Damage>()->faction = 1.f;
    cover->leftShield->GetComponent<Component::Damage>()->removeOnImpact = false;

    cover->rightShield->AddComponent<Component::RelativeTransform>()->parentEntity = cover->node;
    cover->rightShield->GetComponent<Component::Transform>()->position.x = -5.f;
    cover->rightShield->AddComponent<Component::Reflect>();
    cover->rightShield->AddComponent<Component::Collider2DCircle>()->radius = 4.f;
    cover->rightShield->AddComponent<Component::Damage>()->faction = 1.f;
    cover->rightShield->GetComponent<Component::Damage>()->removeOnImpact = false;
}

void ShieldBoss::Activate() {
    SuperBoss::Activate();
}

void ShieldBoss::Deactivate() {
    SuperBoss::Deactivate();
}

void ShieldBoss::mUpdateFunction() {
    SuperBoss::mUpdateFunction();
    for (int i = 0; i < 3; i++) {
        Component::Transform* transform = coverArr[i].node->GetComponent<Component::Transform>();
        transform->Move(transform->GetWorldDirection() * 0.2f * ((float)moveCoverIn * 2.f - 1.f));
    }
    Component::Spawner* spawner = node->GetComponent<Component::Spawner>();
    if (spawner->timeSinceSpawn >= spawner->delay) {
        moveCoverIn = !moveCoverIn;
        spawner->timeSinceSpawn = 0.f;
    }
}
