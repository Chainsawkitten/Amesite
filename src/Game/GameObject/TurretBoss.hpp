#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// A TurretBoss
    class TurretBoss : public SuperGameObject {
        public:
            /// Create %TurretBoss
            /**
             * @param scene Pointer to which Scene %Enemy Enities' are contained.
             */
            TurretBoss(Scene* scene);
           
            ~TurretBoss();

            /// Node of %TurretBoss
            Entity* node;

            /// Body relative to node
           // Entity* body;

        private:
            //Geometry::OBJModel* mShield;
    };
}
