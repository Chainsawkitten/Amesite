#include "DebugDrawingSystem.hpp"

using namespace System;

DebugDrawingSystem* DebugDrawingSystem::mActiveInstance = nullptr;

DebugDrawingSystem::DebugDrawingSystem() {
    mActiveInstance = this;
}

DebugDrawingSystem::~DebugDrawingSystem() {
    mActiveInstance = nullptr;
}

DebugDrawingSystem* DebugDrawingSystem::GetActiveInstance() {
    return mActiveInstance;
}

void DebugDrawingSystem::AddSphere(const glm::vec3& position, float radius, const glm::vec3& color, float lineWidth, float duration, bool depthTesting) {
    Sphere sphere;
    sphere.position = position;
    sphere.radius = radius;
    sphere.color = color;
    sphere.lineWidth = lineWidth;
    sphere.duration = duration;
    sphere.depthTesting = depthTesting;
    mSpheres.push_back(sphere);
}
