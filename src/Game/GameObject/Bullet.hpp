#pragma once

#include <GameObject/SuperGameObject.hpp>

class Scene;
class Entity;
namespace Component {
    class PointLight;
}

namespace GameObject {
    /// A bullet
    class Bullet : public SuperGameObject {
        public:
            /// Create %Bullet
            /**
             * @param scene Pointer to which Scene %Bullet Enities' are contained.
             */
            Bullet(Scene* scene, float lifeTime = 4.f);
           
            ~Bullet();

            /// Node of the %Bullet
            Entity* node;

            Entity* tail;

            /// How bright it shines
            /**
            * @default 1.f
            */
            float lightIntensity;

        private:
            void mUpdateFunction();
    };
}
