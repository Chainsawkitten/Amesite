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
    while (true) {
        {
            std::unique_lock<std::mutex> lock(mThreadPool.mJobMutex);
            
            while (!mThreadPool.mStop && mThreadPool.mJobs.empty()) {
                // Wait for new job to become available.
                mThreadPool.mJobCondition.wait(lock);
            }
            
            if (mThreadPool.mStop)
                return;
            
            // Get next job.
            job = mThreadPool.mJobs.front();
            mThreadPool.mJobs.pop();
        }
        
        // Perform the job.
        job();
        
        // Signal that we finished the job.
        {
            std::unique_lock<std::mutex> lock(mThreadPool.mJobMutex);
            --mThreadPool.mUnfinishedJobs;
        }
        mThreadPool.mFinishedCondition.notify_all();
    }
}
