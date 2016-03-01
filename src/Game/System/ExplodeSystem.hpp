#pragma once

#include <vector>

class Scene;
namespace Audio {
    class SoundBuffer;
}

namespace System {
    /// %System to explode killed entites.
    class ExplodeSystem {
        public:
            /// Create explode system.
            ExplodeSystem();
            
            /// Destructor.
            ~ExplodeSystem();
            
            /// Update Scene containing entities. Explodes killed entites.
            /**
             * @param scene The scene to update.
             */
            void Update(Scene& scene);
            
        private:
            std::vector<Audio::SoundBuffer*> mExplosionSounds;
    };
}
