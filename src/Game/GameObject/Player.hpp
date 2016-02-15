#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// A Player
    class Player : public SuperGameObject {
        public:
            /// Create %Player
            /**
             * @param scene Pointer to which Scene %Player Enities' are contained.
             */
            Player(Scene* scene);
           
            ~Player();
        private:
            Geometry::OBJModel* mModel;
    };
}
