#include "Profiling.hpp"

#include "Log.hpp"
#include <GLFW/glfw3.h>

Profiling::Result Profiling::first("", nullptr);
Profiling::Result* Profiling::current = nullptr;

Profiling::Profiling(const std::string& name) {
    if (Profiling::current == nullptr) {
        first.name = name;
        first.parent = nullptr;
        current = &first;
        mResult = &first;
    } else {
        current->children.push_back(Result(name, current));
        mResult = &current->children.back();
        current = mResult;
    }
    
    mStart = glfwGetTime();
}

Profiling::~Profiling() {
    mResult->duration = glfwGetTime() - mStart;
    if (current == mResult)
        current = mResult->parent;
}

void Profiling::Init() {
    /// @todo Load font and shaders.
}

void Profiling::Free() {
    BeginFrame();
}

void Profiling::BeginFrame() {
    // Clear previous results.
    first.children.clear();
    first.name = "";
    first.duration = 0.0;
    current = nullptr;
}

void Profiling::LogResults() {
    LogResult(first, 0);
}

void Profiling::LogResult(const Result& result, unsigned int indentation) {
    for (unsigned int i=0; i<indentation; ++i)
        Log() << "  ";
    
    Log() << result.name << " " << (result.duration * 1000.0) << " ms\n";
    
    for (const Result& child : result.children)
        LogResult(child, indentation + 1);
}

Profiling::Result::Result(const std::string& name, Result* parent) {
    this->name = name;
    this->parent = parent;
}
