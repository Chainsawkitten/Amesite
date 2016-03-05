#include "ThreadPool.hpp"

#include "Worker.hpp"

using namespace Threading;

ThreadPool::ThreadPool(std::size_t threads) {
    for (std::size_t i = 0; i < threads; ++i)
        mWorkers.push_back(Worker(*this));
}

void ThreadPool::Wait() {
    /// @todo Wait for worker threads to finish their jobs.
}

void ThreadPool::Stop() {
    Wait();
    
    for (std::size_t i = 0; i < mWorkers.size(); ++i)
        mWorkers[i].Join();
}
