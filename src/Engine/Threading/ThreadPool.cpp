#include "ThreadPool.hpp"

#include "Worker.hpp"

using namespace Threading;

ThreadPool::ThreadPool(std::size_t threads) {
    mStop = false;
    mUnfinishedJobs = 0;
    
    for (std::size_t i = 0; i < threads; ++i)
        mWorkers.push_back(new Worker(*this));
}

ThreadPool::~ThreadPool() {
    for (std::size_t i = 0; i < mWorkers.size(); ++i)
        delete mWorkers[i];
}

void ThreadPool::Add(std::function<void()> job) {
    {
        std::unique_lock<std::mutex> lock(mJobMutex);
        mJobs.push(job);
        ++mUnfinishedJobs;
    }
    
    mJobCondition.notify_one();
}

void ThreadPool::Wait() {
    std::unique_lock<std::mutex> lock(mJobMutex);
    
    while (mUnfinishedJobs > 0)
        mFinishedCondition.wait(lock);
}

void ThreadPool::Stop() {
    Wait();
    
    mStop = true;
    mJobCondition.notify_all();
    
    for (std::size_t i = 0; i < mWorkers.size(); ++i)
        mWorkers[i]->Join();
}
