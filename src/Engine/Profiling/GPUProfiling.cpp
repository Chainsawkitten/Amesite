#include "GPUProfiling.hpp"

#include <assert.h>
#include "../Util/Log.hpp"
#include "Query.hpp"

GPUProfiling::GPUProfiling(const std::string& name) : active(false) {
    // Check if profiling.
    if (Profiling().IsActive())
        active = true;
    
    // Start profiling if active.
    if (active)
        result = Profiling().StartResult(name, ProfilingManager::GPU_TIME);
}

GPUProfiling::~GPUProfiling() {
    if (active)
        Profiling().FinishResult(result, ProfilingManager::GPU_TIME);
}
