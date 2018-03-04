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
