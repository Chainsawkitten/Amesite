#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// A Pylon
    class Pylon : public SuperGameObject {
        public:
            /// Create %Pylon
            /**
             * @param scene Pointer to which Scene %Enemy Enities' are contained.
             */
            Pylon(Scene* scene);
           
            ~Pylon();

            /// Node of %Pylon
            Entity* node;
            /// Body relative to Node
            Entity* body;
            /// Pylon1 relative to Node
            Entity* pylon1;
            /// Pylon2 relative to Node
            Entity* pylon2;
            /// Pylon3 relative to Node
            Entity* pylon3;
            /// Turrent relative to Body
            Entity* turrent;

        private:
            Geometry::OBJModel* mBody;
            Geometry::OBJModel* mPylon1;
            Geometry::OBJModel* mPylon2;

            void AddPylonPartilces(Entity* entity);
    };
}
