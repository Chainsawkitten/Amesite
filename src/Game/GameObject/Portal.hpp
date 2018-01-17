#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// Portal
    class Portal : public SuperGameObject {
        public:
            /// Create %Portal
            /**
             * @param scene Pointer to which Scene %Enemy Enities' are contained.
             */
            Portal(Scene* scene);
           
            ~Portal();

            /// Node of %Portal
            Entity* node;

        private:
            Geometry::OBJModel* mPortalModel;
    };
}
