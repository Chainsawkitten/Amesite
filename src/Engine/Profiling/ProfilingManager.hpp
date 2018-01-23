#pragma once

#include <string>
#include <list>
#include <map>

class Font;
namespace Geometry {
    class Square;
}

/// Handles profiling.
class ProfilingManager {
    friend class Hub;
    friend class CPUProfiling;
    
    public:
        /// The type of profiling to perform.
        enum Type {
            CPU_TIME = 0,
            COUNT
        };
        
        /// A profiling result.
        struct Result {
            std::string name;
            double duration = 0.0;
            std::list<Result> children;
            Result* parent;
            
            Result(const std::string& name, Result* parent);
        };
        
        /// Get instance of profiling manager.
        /**
         * @return The profiling manager instance.
         */
        static ProfilingManager& GetInstance();
        
        /// Init the profiling manager.
        void Init();
        
        /// Free resources allocated by the profiling manager.
        void Free();
        
        /// Begin profiling a frame.
        void BeginFrame();
        
        /// End profiling a frame and collect the results.
        void EndFrame();
        
        /// Check whether %ProfilingManager is active.
        /**
         * @return Active state.
         */
        bool IsActive() const;
        
        /// Set whether %ProfilingManager is active.
        /**
         * @param active Active state.
         */
        void SetActive(bool active);
        
        /// Log the results of the profiling.
        void LogResults();
        
        /// Draw the results of the profiling.
        void DrawResults();
        
    private:
        ProfilingManager();
        ~ProfilingManager();
        ProfilingManager(ProfilingManager const&) = delete;
        void operator=(ProfilingManager const&) = delete;
        
        Result* StartResult(const std::string& name, Type type);
        void FinishResult(Result* result, Type type);
        
        void LogResult(const Result& result, unsigned int indentation);
        void DrawResult(Result& result, float x, float& y);
        static std::string TypeToString(Type type);
        
        bool active;
        
        Result* root[Type::COUNT];
        Result* current[Type::COUNT];
        
        double frameStart;
        
        Font* font;              
        Geometry::Square* square;
};

ProfilingManager& Profiling();
