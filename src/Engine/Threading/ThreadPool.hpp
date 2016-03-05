#pragma once

#include <vector>

namespace Threading {
    class Worker;
    
    /// Handles a pool of worker threads.
    class ThreadPool {
        public:
            /// Create a new thread pool.
            /**
             * @param threads Number of threads in the pool.
             */
            ThreadPool(std::size_t threads);
            
            /// Destructor.
            ~ThreadPool();
            
            /// Wait for the worker threads to finish.
            void Wait();
            
            /// Wait for jobs to finish, the stop the worker threads.
            void Stop();
            
        private:
            std::vector<Worker*> mWorkers;
    };
}
