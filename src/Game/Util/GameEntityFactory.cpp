#include "GameEntityFactory.hpp"
#include <Entity/Entity.hpp>
#include <Resources.hpp>
#include <Component/Mesh.hpp>
#include <Component/Lens.hpp>
#include <Component/Transform.hpp>
#include <Component/RelativeTransform.hpp>
#include <Component/Collider2DCircle.hpp>
#include <Component/Collider2DRectangle.hpp>
#include <Component/Physics.hpp>
#include <Component/SpotLight.hpp>
#include <Component/ParticleEmitter.hpp>
#include "../Component/Controller.hpp"
#include "../Component/Damage.hpp"

#include <Geometry/Geometry3D.hpp>
#include <Geometry/Cube.hpp>
#include "../CaveSystem/CaveSystem.hpp"

#include <Texture/Texture2D.hpp>

#include <Scene/Scene.hpp>

#include <../Game/Util/ControlSchemes.hpp>

GameEntityFactory& GameEntityFactory::GetInstance() {
    static GameEntityFactory instance;

    return instance;
}

GameEntityFactory::GameEntityFactory(){
    mScene = nullptr;
}

Entity* GameEntityFactory::CreateBasicEnemy( glm::vec3 origin ) {
    Entity* enemyEntity = mScene->CreateEntity();
    enemyEntity->AddComponent<Component::Mesh>();
    enemyEntity->AddComponent<Component::Transform>();
    enemyEntity->AddComponent<Component::Collider2DCircle>();

    enemyEntity->GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();
    enemyEntity->GetComponent<Component::Transform>()->position = origin;
    enemyEntity->GetComponent<Component::Collider2DCircle>()->radius = 0.5f;

    return enemyEntity;
}

Entity* GameEntityFactory::CreatePlayer(glm::vec3 origin, InputHandler::Player player) {
    
    Entity* playerEntity = mScene->CreateEntity();
    playerEntity->AddComponent<Component::Mesh>();
    playerEntity->AddComponent<Component::Transform>();
    playerEntity->AddComponent<Component::Collider2DCircle>();
    playerEntity->AddComponent<Component::Physics>();
    playerEntity->AddComponent<Component::Controller>();
    playerEntity->AddComponent<Component::Damage>();

    playerEntity->GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();
    playerEntity->GetComponent<Component::Transform>()->position = origin;
    playerEntity->GetComponent<Component::Collider2DCircle>()->radius = 0.5f;
    playerEntity->GetComponent<Component::Controller>()->playerID = player;

    playerEntity->GetComponent<Component::Damage>()->damageAmount = 10.f;

    playerEntity->GetComponent<Component::Controller>()->ControlScheme = &ControlScheme::StickMove;

    return playerEntity;
}

Entity* GameEntityFactory::CreateCube( glm::vec3 origin ) {
	Entity* cubeEntity = mScene->CreateEntity();
	cubeEntity->AddComponent<Component::Mesh>();
	cubeEntity->AddComponent<Component::Transform>();

	cubeEntity->GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();
	cubeEntity->GetComponent<Component::Transform>()->position = origin;

	return cubeEntity;
}

Entity* GameEntityFactory::CreateCamera( glm::vec3 origin, glm::vec3 rotation ) {
    Entity* cameraEntity = mScene->CreateEntity();
    cameraEntity->AddComponent<Component::Transform>();
    cameraEntity->AddComponent<Component::Lens>();

    cameraEntity->GetComponent<Component::Transform>()->position = origin;
    cameraEntity->GetComponent<Component::Transform>()->Rotate( rotation.x, rotation.y, rotation.z );

    return cameraEntity;
}

void GameEntityFactory::SetScene( Scene* scene ) {
    mScene = scene;
}

void GameEntityFactory::CreateCuboidParticle(Entity * camera, Texture2D* particleTexture)
{
    camera->AddComponent<Component::ParticleEmitter>();

    // Particle emitter.
    Component::ParticleEmitter* emitter = camera->GetComponent<Component::ParticleEmitter>();

    emitter->emitterType = Component::ParticleEmitter::CUBOID;
    emitter->follow = camera;
    emitter->maxEmitTime = 0.02;
    emitter->minEmitTime = 0.01;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 0.0;
    emitter->origin = glm::vec3(0.f, 0.f, 0.f);
    emitter->size = glm::vec3(40.f, 15.f, 40.f);
    emitter->relative = true;
    emitter->follow = camera;
    emitter->timeToNext = 5.0;

    //Particle type.
    emitter->particleType.texture = particleTexture;
    emitter->particleType.minLifetime = 6.f;
    emitter->particleType.maxLifetime = 10.f;
    emitter->particleType.minVelocity = glm::vec3(-0.025f, -0.01f, -0.025f);
    emitter->particleType.maxVelocity = glm::vec3(0.025f, -0.1f, 0.025f);
    emitter->particleType.minSize = glm::vec2(0.025f, 0.025f);
    emitter->particleType.maxSize = glm::vec2(0.05f, 0.05f);
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(.3f, .3f, 1.f);
}

Entity* GameEntityFactory::CreateMap() {
    Caves::CaveSystem cave = Caves::CaveSystem();
    return cave.GenerateCaveSystem(mScene);
}

GameEntityFactory::~GameEntityFactory() {

}

GameEntityFactory& GameEntityCreator() {
    return GameEntityFactory::GetInstance();
}
