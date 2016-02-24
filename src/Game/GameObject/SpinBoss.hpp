#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

#include <glm/glm.hpp>
#include <vector>

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// A SpinBoss
    class SpinBoss : public SuperGameObject {
        public:
            /// Create %SpinBoss
            /**
             * @param scene Pointer to which Scene %Enemy Enities' are contained.
             */
            SpinBoss(Scene* scene);
           
            ~SpinBoss();

            /// Node of %SpinBoss
            Entity* node;

            Entity* body;

            Entity* armArr[4];

            void Update();

            void Reset();

        private:
            void CreateArm(Entity* entity, glm::vec3 direction);
            Geometry::OBJModel* mBody;
            bool firstPhase;
    };
}
