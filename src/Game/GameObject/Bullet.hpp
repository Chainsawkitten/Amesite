#pragma once

#include <GameObject/SuperGameObject.hpp>

class Scene;
class Entity;
namespace Component {
    class PointLight;
}

namespace GameObject {
    /// A bullet which does damage on impact.
    class Bullet : public SuperGameObject {
        public:
            /// Create %Bullet
            /**
             * @param scene Pointer to which Scene %Bullet Enities' are contained.
             * @param lifeTime How long to live (in seconds).
             */
            Bullet(Scene* scene, float lifeTime = 4.f);
            
            /// Destructor.
            ~Bullet();
            
            /// Node of the %Bullet
            Entity* node;
            
            /// Tail node.
            Entity* tail;

            /// How bright it shines
            /**
             * Default: 1.
             */
            float lightIntensity;
            
        private:
            void mUpdateFunction();
    };
}
