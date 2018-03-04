#pragma once

namespace System {
    /// Handles debug geometry.
    class DebugDrawingSystem {
        public:
            /// Creates the debug drawing system.
            DebugDrawingSystem();
            
            /// Destructor.
            ~DebugDrawingSystem();
            
            /// Get currently active debug drawing system.
            /**
             * @return The currently active debug drawing system, or nullptr.
             */
            static DebugDrawingSystem* GetActiveInstance();
            
        private:
            static DebugDrawingSystem* mActiveInstance;
    };
}
