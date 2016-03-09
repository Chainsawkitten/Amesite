#pragma once

#include "SuperBoss.hpp"
#include <random>

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// A DivideBoss
    class DivideBoss : public SuperBoss {
        public:
            /// Create %DivideBoss
            /**
             * @param scene Pointer to which Scene %DivideBoss Enities' are contained.
             */
            DivideBoss(Scene* scene);
           
            ~DivideBoss();

            /// Gets position of boss.
            /**
             * @return Position of the boss.
             */
            glm::vec3 GetPosition();

            /// Activates the enemy.
            void Activate();

            /// Deactivates the enemy.
            void Deactivate();

            /// Gets health of the boss.
            /**
             * @return Health of the boss.
             */
            float GetHealth();
        protected:
            void mUpdateFunction();
        private:
            Geometry::OBJModel* mBodyModel;

            float mNextHpStep;

            std::mt19937 mRNG;
    };
}
