#include "ThreadPool.hpp"

#include "Worker.hpp"

using namespace Threading;

ThreadPool::ThreadPool(std::size_t threads) {
    for (std::size_t i = 0; i < threads; ++i)
        mWorkers.push_back(new Worker(*this));
}

ThreadPool::~ThreadPool() {
    for (std::size_t i = 0; i < mWorkers.size(); ++i)
        delete mWorkers[i];
}

void ThreadPool::Wait() {
    for (std::size_t i = 0; i < mWorkers.size(); ++i)
        mWorkers[i]->Wait();
}

void ThreadPool::Stop() {
    Wait();
    
    for (std::size_t i = 0; i < mWorkers.size(); ++i)
        mWorkers[i]->Join();
}
