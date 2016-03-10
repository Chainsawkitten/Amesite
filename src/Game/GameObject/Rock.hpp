#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

#include <glm/glm.hpp>

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// Rock
    class Rock : public SuperGameObject {
        public:
            /// Create %Pillar
            /**
             * @param scene Pointer to which Scene %Enemy Enities' are contained.
             * @param bossPosition Vector of boss position.
             */
            Rock(Scene* scene);
           
            ~Rock();

            /// Node of %Rock
            Entity* node;

        private:

            Geometry::OBJModel* mPillarModel;

            Entity* mLight;

    };
}
