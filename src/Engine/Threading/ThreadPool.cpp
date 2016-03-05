#include "ThreadPool.hpp"

#include "Worker.hpp"

using namespace Threading;

ThreadPool::ThreadPool(std::size_t threads) {
    for (std::size_t i = 0; i<threads; ++i) {
        mWorkers.push_back(std::thread(Worker(*this)));
    }
}
