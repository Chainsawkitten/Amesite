#pragma once

#include <GameObject/SuperGameObject.hpp>

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// A Shield
    class Shield : public SuperGameObject {
        public:
            /// Create %Shield
            /**
             * @param scene Pointer to which Scene %Enemy Enities' are contained.
             */
            Shield(Scene* scene);
           
            ~Shield();

            /// Node of %Shield
            Entity* node;

            /// Body relative to node
            Entity* body;

        private:
            Geometry::OBJModel* mShield;
    };
}
