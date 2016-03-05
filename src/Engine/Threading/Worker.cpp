#include "Worker.hpp"

#include <thread>
#include <functional>

using namespace Threading;

Worker::Worker(ThreadPool& threadPool) : mThreadPool(threadPool) {
    mFinished = false;
    mThread = new std::thread(std::bind(&Worker::Execute, this));
}

void Worker::Wait() {
    std::unique_lock<std::mutex> lock(mFinishedMutex);
    
    while (!mFinished) {
        mFinishedCondition.wait(lock);
    }
}

void Worker::Join() {
    mThread->join();
}

void Worker::Execute() {
    /// @todo Implement worker thread.
    
    // Signal that we've finished all our jobs (for now).
    mFinished = true;
    mFinishedCondition.notify_all();
}
