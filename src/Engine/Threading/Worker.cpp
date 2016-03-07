#include "Worker.hpp"

#include "ThreadPool.hpp"
#include <thread>

using namespace Threading;

Worker::Worker(ThreadPool& threadPool) : mThreadPool(threadPool) {
    mThread = new std::thread(std::bind(&Worker::Execute, this));
}

void Worker::Join() {
    mThread->join();
}

void Worker::Execute() {
    std::function<void()> job;
    std::unique_lock<std::mutex> lock(mThreadPool.mJobMutex, std::defer_lock);
    
    while (true) {
        lock.lock();
        
        // Wait for new job to become available.
        while (!mThreadPool.mStop && mThreadPool.mJobs.empty())
            mThreadPool.mJobCondition.wait(lock);
        
        if (mThreadPool.mStop)
            return;
        
        // Get next job.
        job = mThreadPool.mJobs.front();
        mThreadPool.mJobs.pop();
        
        lock.unlock();
        
        // Perform the job.
        job();
        
        // Signal that we finished the job.
        lock.lock();
        --mThreadPool.mUnfinishedJobs;
        mThreadPool.mFinishedCondition.notify_all();
        lock.unlock();
    }
}
