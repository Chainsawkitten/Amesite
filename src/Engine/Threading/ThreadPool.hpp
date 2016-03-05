#pragma once

#include <vector>
#include <thread>

namespace Threading {
    /// Handles a pool of worker threads.
    class ThreadPool {
        public:
            /// Create a new thread pool.
            /**
             * @param threads Number of threads in the pool.
             */
            ThreadPool(std::size_t threads);
            
        private:
            std::vector<std::thread> mWorkers;
    };
}
