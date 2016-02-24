#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// A TurrentBoss
    class TurrentBoss : public SuperGameObject {
        public:
            /// Create %TurrentBoss
            /**
             * @param scene Pointer to which Scene %Enemy Enities' are contained.
             */
            TurrentBoss(Scene* scene);
           
            ~TurrentBoss();

            /// Node of %TurrentBoss
            Entity* node;

            /// Body relative to node
           // Entity* body;

        private:
            //Geometry::OBJModel* mShield;
    };
}
