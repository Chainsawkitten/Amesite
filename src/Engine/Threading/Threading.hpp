#pragma once

#include "ThreadPool.hpp"

/// Multi-threading functionality.
namespace Threading {
    /// Get number of threads that can run in parallel.
    /**
     * @return The number of threads that can run concurrently.
     */
    unsigned int GetParallelCount();
}
