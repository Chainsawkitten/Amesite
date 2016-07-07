#pragma once

/// %Profiling functionality.
namespace Profiling {
    /// Initialize profiling functionality.
    void Init();
    
    /// Free profiling resources.
    void Free();
    
    /// Begin profiling a frame.
    void BeginFrame();
    
    /// Log the results of the profiling.
    void LogResults();
}
