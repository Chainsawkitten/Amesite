#include "LifeTimeSystem.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include <GameObject/SuperGameObject.hpp>

#include "../Util/GameEntityFactory.hpp"

#include "../Component/Explode.hpp"
#include "../Component/LifeTime.hpp"

#include <Threading/Threading.hpp>

using namespace System;

LifeTimeSystem::LifeTimeSystem() {
}

LifeTimeSystem::~LifeTimeSystem() {
}

void LifeTimeSystem::Update(Scene& scene, float deltaTime) {
    std::vector<Component::LifeTime*>& lifeTimes = scene.GetAll<Component::LifeTime>();
    
    unsigned int threads = Threading::GetParallelCount();
    std::size_t length = lifeTimes.size() / threads;
    for (unsigned int i=0; i < threads; ++i) {
        std::size_t begin = i * length;
        if (i == threads - 1)
            length = lifeTimes.size() - begin;
        
        if (length > 0)
            Threading::FrontEndJobs().Add(std::bind(&LifeTimeSystem::UpdatePart, this, std::ref(lifeTimes), deltaTime, begin, length));
    }
    
    Threading::FrontEndJobs().Wait();
}

void LifeTimeSystem::UpdatePart(std::vector<Component::LifeTime*>& lifeTimes, float deltaTime, std::size_t begin, std::size_t length) {
    for (std::size_t i=begin; i < begin+length; ++i) {
        lifeTimes[i]->lifeTime -= deltaTime;
        if (lifeTimes[i]->lifeTime < 0.f)
            if (lifeTimes[i]->entity->gameObject != nullptr)
                lifeTimes[i]->entity->gameObject->Kill();
            else
                lifeTimes[i]->entity->Kill();
    }
}
