#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>

namespace std {
    class thread;
}

namespace Threading {
    class ThreadPool;
    
    /// Worker thread. Gets jobs from the thread pool and executes them.
    class Worker {
        public:
            /// Create new worker thread.
            /**
             * @param threadPool Thread pool this thread belongs to.
             */
            Worker(ThreadPool& threadPool);
            
            /// Wait for the worker thread to finish.
            void Wait();
            
            /// Join the thread.
            void Join();
            
        private:
            ThreadPool& mThreadPool;
            
            std::thread* mThread;
            
            std::atomic<bool> mFinished;
            std::mutex mFinishedMutex;
            std::condition_variable mFinishedCondition;
            
            // Execute the worker thread. Continually checks for available jobs in the thread pool and executes them.
            void Execute();
    };
}
