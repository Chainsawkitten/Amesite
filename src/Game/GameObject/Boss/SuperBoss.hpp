#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

class Scene;

namespace GameObject {
    /// A SuperBoss
    class SuperBoss : public SuperGameObject {
        public:
            /// Create %SuperBoss
            /**
             * @param scene Pointer to which Scene %Enemy Enities' are contained.
             */
            SuperBoss(Scene* scene);
           
            virtual ~SuperBoss();

            /// Node of %SuperBoss, contains health and transform
            Entity* node;

            /// Body relative to node
            Entity* body;
    };
}
