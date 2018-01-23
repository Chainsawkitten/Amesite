#include "CPUProfiling.hpp"

#include <GLFW/glfw3.h>

using namespace std;

CPUProfiling::CPUProfiling(const std::string& name) {
    if (Profiling().IsActive()) {
        result = Profiling().StartResult(name, ProfilingManager::Type::CPU_TIME);
        start = glfwGetTime();
    }
}

CPUProfiling::~CPUProfiling() {
    if (Profiling().IsActive()) {
        result->duration = (glfwGetTime() - start) * 1000.0;
        Profiling().FinishResult(result, ProfilingManager::Type::CPU_TIME);
    }
}
