#include "Worker.hpp"

using namespace Threading;

Worker::Worker(ThreadPool& threadPool) : mThreadPool(threadPool) {
    
}

void Worker::operator()() {
    /// @todo Implement worker thread.
}
