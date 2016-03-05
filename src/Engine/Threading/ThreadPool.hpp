#pragma once

#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

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
            
            /// Add a job to be peformed by the thread pool.
            /**
             * @param job Job to perform.
             */
            void Add(std::function<void()> job);
            
            /// Wait for the worker threads to finish.
            void Wait();
            
            /// Wait for jobs to finish, then stop the worker threads.
            void Stop();
            
        private:
            friend class Worker;
            
            // Worker threads.
            std::vector<Worker*> mWorkers;
            
            // Job queue, worker threads take jobs from here.
            std::queue<std::function<void()>> mJobs;
            std::mutex mJobMutex;
            std::condition_variable mJobCondition;
            
            std::size_t mUnfinishedJobs;
            std::condition_variable mFinishedCondition;
            
            // Whether worker threads should stop.
            std::atomic<bool> mStop;
    };
}
