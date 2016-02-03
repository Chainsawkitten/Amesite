#include "CaveSystem.hpp"
#include <Geometry/Cube.hpp>
#include <Resources.hpp>
#include <Component/Mesh.hpp>
#include <Component/Transform.hpp>
#include <Component/RelativeTransform.hpp>

using namespace Caves;

CaveSystem::CaveSystem(Scene* scene) {
    mScene = scene;
}

CaveSystem::~CaveSystem() {
}

	Entity* CaveSystem::GenerateCaveSystem() {
	return nullptr;
	//for (int i = 0; i < 25; i++) {
	//    for (int j = 0; j < 25; j++) {
	//        if(mMap[i][j] > 0.f){
	//            Entity* wall = mScene->CreateEntity();
	//            wall->AddComponent<Component::Transform>();
	//            wall->AddComponent<Component::Mesh>();
	//            wall->GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();
	//            wall->GetComponent<Component::Transform>()->position = glm::vec3(float(j), -float(i), 0.f);
	//        }
	//    }
	//}
}