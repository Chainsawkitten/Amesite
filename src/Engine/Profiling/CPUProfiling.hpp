#pragma once

#include <string>
#include "ProfilingManager.hpp"

/// Run profiling (CPU).
class CPUProfiling {
    public:
        /// Start profiling.
        /**
         * @param name Name of the segment.
         */
        explicit CPUProfiling(const std::string& name);
        
        /// End profiling.
        ~CPUProfiling();
        
    private:
        ProfilingManager::Result* result;
        double start;
};

#define PROFILE_CPU(name) CPUProfiling __profileInstance(name)
