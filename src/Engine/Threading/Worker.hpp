#pragma once

#include <atomic>

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
            
            /// Join the thread.
            void Join();
            
        private:
            ThreadPool& mThreadPool;
            
            std::thread* mThread;
            
            std::atomic<bool> mFinished;
            
            // Execute the worker thread. Continually checks for available jobs in the thread pool and executes them.
            void Execute();
    };
}
