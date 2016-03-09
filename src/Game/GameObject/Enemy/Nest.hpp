#pragma once

#include "SuperEnemy.hpp"

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

class Texture2D;

namespace GameObject {
    /// A Nest
    class Nest : public SuperEnemy {
        public:
            /// Create %Nest
            /**
             * @param scene Pointer to which Scene %Nest Enities' are contained.
             */
            Nest(Scene* scene);
           
            ~Nest();

            /// Body relative to Node
            Entity* body;

            /// Activates the enemy.
            void Activate();

            /// Deactivates the enemy.
            void Deactivate();

            /// Gets health of the enemy.
            /**
             * @return health of the enemy.
             */
            float GetHealth();
        protected:
            void mUpdateFunction();
        private:
            Geometry::OBJModel* mBodyModel;

            Texture2D* mActiveGlowBody;
            Texture2D* mDeactiveGlowBody;
    };
}
