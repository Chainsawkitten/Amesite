#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// Altar
    class Altar : public SuperGameObject {
        public:
            /// Create %Altar
            /**
             * @param scene Pointer to which Scene %Enemy Enities' are contained.
             */
            Altar(Scene* scene);
           
            ~Altar();

            /// Node of %Altar
            Entity* node;

        private:
            Geometry::OBJModel* mAltarModel;
            Geometry::OBJModel* mPillarModel;
    };
}
