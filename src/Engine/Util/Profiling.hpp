#pragma once

#include <string>
#include <list>

/// Run profiling.
class Profiling {
    public:
        /// Start profiling.
        /**
         * @param name Name of the segment.
         */
        Profiling(const std::string& name);
        
        /// End profiling.
        ~Profiling();
        
        /// Initialize profiling functionality.
        static void Init();
        
        /// Free profiling resources.
        static void Free();
        
        /// Begin profiling a frame.
        static void BeginFrame();
        
        /// Log the results of the profiling.
        static void LogResults();
        
    private:
        struct Result {
            std::string name;
            double duration;
            std::list<Result> children;
            
            Result(const std::string& name);
        };
        
        Result* mResult;
        double mStart;
        
        static Result first;
        static Result* current;
};

#define PROFILE(name) Profiling _p(name)
