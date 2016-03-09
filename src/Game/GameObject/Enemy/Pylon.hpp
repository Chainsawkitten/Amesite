#pragma once

#include "SuperEnemy.hpp"

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

class Texture2D;

namespace GameObject {
    /// A Pylon
    class Pylon : public SuperEnemy {
        public:
            /// Create %Pylon
            /**
             * @param scene Pointer to which Scene %Pylon Enities' are contained.
             */
            Pylon(Scene* scene);
           
            ~Pylon();

            /// Body relative to Node
            Entity* body;
            /// Pylon1 relative to Node
            Entity* pylon1;
            /// Pylon2 relative to Node
            Entity* pylon2;
            /// Pylon3 relative to Node
            Entity* pylon3;
            /// Turret relative to Body
            Entity* turret;

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
            Geometry::OBJModel* mPylon1;
            Geometry::OBJModel* mPylon2;

            Texture2D* mActiveGlowBody;
            Texture2D* mDeactiveGlowBody;
            Texture2D* mActiveGlowPylon1;
            Texture2D* mDeactiveGlowPylon1;
            Texture2D* mActiveGlowPylon2;
            Texture2D* mDeactiveGlowPylon2;

            void AddPylonPartilces(Entity* entity);
    };
}
