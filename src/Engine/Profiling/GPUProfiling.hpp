#pragma once

#include "ProfilingManager.hpp"
#include <string>

/// Run gpu profiling.
class GPUProfiling {
    public:
        /// Start gpu profiling.
        /**
         * @param name Name of the segment.
         */
        GPUProfiling(const std::string& name);
        
        /// End gpu profiling.
        ~GPUProfiling();
        
    private:
        ProfilingManager::Result* result;
        bool active;
};

#define PROFILE_GPU(name) GPUProfiling __gpuProfileInstance(name)
