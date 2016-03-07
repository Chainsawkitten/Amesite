#pragma once

#include "ThreadPool.hpp"
#include "../Util/Log.hpp"

/// Multi-threading functionality.
namespace Threading {
    /// Get number of threads that can run in parallel.
    /**
     * @return The number of threads that can run concurrently.
     */
    unsigned int GetParallelCount();
    
    /// Get thread pool for front end jobs.
    /**
     * @return Thread pool responsible for front end jobs.
     */
    ThreadPool& FrontEndJobs();
    
    /// Loop over a vector concurrently.
    /**
     * Work will be split over as many tasks as can run concurrently on the current system.
     * @param method Method to execute.
     * @param object Object to execute the method on.
     * @param list List to loop over.
     */
    template<typename M, typename O, typename T>
    void ParallelFor(M method, O* object, std::vector<T>& list);
}

template<typename M, typename O, typename T>
void Threading::ParallelFor(M method, O* object, std::vector<T>& list) {
    Log () << "Parallel for (list size " << std::to_string(list.size()) << "):\n";
    unsigned int threads = GetParallelCount();
    std::size_t length = list.size() / threads;
    for (unsigned int i=0; i < threads; ++i) {
        std::size_t begin = i * length;
        if (i == threads - 1)
            length = list.size() - begin;
        
        Log() << "Task " << std::to_string(i) << ": " << std::to_string(begin) << ", " << std::to_string(length) << "\n";
        
        if (length > 0)
            Threading::FrontEndJobs().Add(std::bind(method, object, std::ref(list), begin, length));
    }
}
