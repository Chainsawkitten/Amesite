#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// Spawn
    class Spawn : public SuperGameObject {
        public:
            /// Create %Spawn
            /**
             * @param scene Pointer to which Scene %Enemy Enities' are contained.
             */
            Spawn(Scene* scene);
           
            ~Spawn();

            /// Node of %Spawn
            Entity* node;

            /// Altar relative to node
            Entity* altar;

            /// Pillar relative to Node
            Entity* pillar1;
            
            /// Pillar relative to Node
            Entity* pillar2;

        private:
            Geometry::OBJModel* mAltarModel;
            Geometry::OBJModel* mPillarModel;
    };
}
