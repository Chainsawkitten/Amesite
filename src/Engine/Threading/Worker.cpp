#include "Worker.hpp"

#include <thread>

using namespace Threading;

Worker::Worker(ThreadPool& threadPool) : mThreadPool(threadPool) {
    mThread = new std::thread(*this);
}

void Worker::Join() {
    mThread->join();
}

void Worker::operator()() {
    /// @todo Implement worker thread.
}
