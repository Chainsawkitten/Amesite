#include "Threading.hpp"

#include <thread>

namespace Threading {
    unsigned int GetParallelCount() {
        unsigned int n = std::thread::hardware_concurrency();
        return n > 0 ? n : 1;
    }
}
