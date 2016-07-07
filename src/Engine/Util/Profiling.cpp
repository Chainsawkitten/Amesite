#include "Profiling.hpp"

#include "Log.hpp"
#include <GLFW/glfw3.h>

Profiling::Result Profiling::first("");
Profiling::Result* Profiling::current = nullptr;

Profiling::Profiling(const std::string& name) {
    if (Profiling::current == nullptr) {
        first.name = name;
        current = &first;
        mResult = &first;
    } else {
        current->children.push_back(Result(name));
        mResult = &current->children.back();
        current = mResult;
    }
    
    mStart = glfwGetTime();
}

Profiling::~Profiling() {
    mResult->duration = glfwGetTime() - mStart;
}

void Profiling::Init() {
    /// @todo Load font and shaders.
}

void Profiling::Free() {
    BeginFrame();
}

void Profiling::BeginFrame() {
    // Clear previous results.
    if (current != nullptr) {
        current->children.clear();
        current = nullptr;
    }
}

void Profiling::LogResults() {
    if (current != nullptr) {
        Log() << current->name << " " << (current->duration * 1000) << " ms\n";
    }
}

Profiling::Result::Result(const std::string& name) {
    this->name = name;
}
