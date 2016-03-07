#include "LifeTimeSystem.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include <GameObject/SuperGameObject.hpp>

#include "../Util/GameEntityFactory.hpp"

#include "../Component/Explode.hpp"
#include "../Component/LifeTime.hpp"

#include <Threading/Threading.hpp>
#include <Util/Log.hpp>

using namespace System;

LifeTimeSystem::LifeTimeSystem() {
}

LifeTimeSystem::~LifeTimeSystem() {
}

void LifeTimeSystem::Update(Scene& scene, float deltaTime) {
    std::vector<Component::LifeTime*>& lifeTimes = scene.GetAll<Component::LifeTime>();
    
    Log() << "Life time\n";
    
    mDeltaTime = deltaTime;
    Threading::ParallelFor(&LifeTimeSystem::UpdatePart, this, lifeTimes);
    Threading::FrontEndJobs().Wait();
}

void LifeTimeSystem::UpdatePart(std::vector<Component::LifeTime*>& lifeTimes, std::size_t begin, std::size_t length) {
    for (std::size_t i=begin; i < begin+length; ++i) {
        lifeTimes[i]->lifeTime -= mDeltaTime;
        if (lifeTimes[i]->lifeTime < 0.f)
            if (lifeTimes[i]->entity->gameObject != nullptr)
                lifeTimes[i]->entity->gameObject->Kill();
            else
                lifeTimes[i]->entity->Kill();
    }
}
