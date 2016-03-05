#include "Worker.hpp"

#include <thread>
#include <functional>

using namespace Threading;

Worker::Worker(ThreadPool& threadPool) : mThreadPool(threadPool) {
    mFinished = false;
    mThread = new std::thread(std::bind(&Worker::Execute, this));
}

void Worker::Join() {
    mThread->join();
}

void Worker::Execute() {
    /// @todo Implement worker thread.
    
    mFinished = true;
}
