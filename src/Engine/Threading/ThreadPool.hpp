#pragma once

#include "Worker.hpp"
#include <vector>

namespace Threading {
    /// Handles a pool of worker threads.
    class ThreadPool {
        public:
            /// Create a new thread pool.
            /**
             * @param threads Number of threads in the pool.
             */
            ThreadPool(std::size_t threads);
            
            /// Wait for the worker threads to finish.
            void Wait();
            
            /// Wait for jobs to finish, the stop the worker threads.
            void Stop();
            
        private:
            std::vector<Worker> mWorkers;
    };
}
