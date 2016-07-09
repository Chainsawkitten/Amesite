#pragma once

#include <Component/SuperComponent.hpp>
#include <vector>
#include <glm/glm.hpp>

namespace Component {
    /// %Component providing AI which walks between a set path.
    class WalkerAI : public SuperComponent {
        public:
            /// Create new walking AI.
            /**
             * @param entity Pointer to which entity this component corresponds.
             */
            WalkerAI(Entity* entity);
            
            /// Destructor.
            ~WalkerAI();

            /// How close is close enough.
            float mThreshold;

            /// How fast do we move.
            float mSpeed;

            /// Updates the walker AI.
            /**
             * @param deltaTime Time since last frame (in seconds).
             */
            void Update(float deltaTime);

            /// Adds a point to the path.
            /**
             * @param point The point to add.
             */
            void AddPoint(glm::vec3 point);

        private:
            // The index of the point we're targeting.
            unsigned int mTarget;

            // The points to travel through.
            std::vector<glm::vec3> mPoints;

            // The direction we are moving through the points.
            bool mForward;

    };
}
