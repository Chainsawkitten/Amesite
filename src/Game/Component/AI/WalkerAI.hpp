#pragma once

#include <Component/SuperComponent.hpp>
#include <vector>
#include <glm/glm.hpp>

namespace Component {
    /// %Component providing player control.
    class WalkerAI : public SuperComponent {
        public:
            /// Create new %Controller.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            WalkerAI(Entity* entity);
            
            /// Destructor.
            ~WalkerAI();

            ///How close is close enough.
            float mThreshold;

            ///How fast do we move.
            float mSpeed;

            /// Updates the walker AI.
            /**
            * @param The delta time
            */
            void Update(float timeDelta);

            ///Adds a point to the path.
            /**
             * @param The point to add.
             */
            void AddPoint(glm::vec3 point);

        private:
            //The index of the point we're targeting.
            int mTarget;

            //The points to travel through.
            std::vector<glm::vec3> mPoints;

            //The direction we are moving through the points.
            bool mForward;

    };
}
