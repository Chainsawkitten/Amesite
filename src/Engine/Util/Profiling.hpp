#pragma once

#include <string>
#include <list>

class Font;
namespace Geometry {
    class Square;
}

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
        
        /// Draw the results of the profiling.
        static void DrawResults();
        
    private:
        struct Result {
            std::string name;
            double duration;
            std::list<Result> children;
            Result* parent;
            
            Result(const std::string& name, Result* parent);
        };
        
        static void LogResult(const Result& result, unsigned int indentation);
        static void DrawResult(const Result& result, float x, float& y);
        
        Result* mResult;
        double mStart;
        
        static Result first;
        static Result* current;
        
        static Font* font;
        static Geometry::Square* square;
};

#define PROFILE(name) Profiling _p(name)
