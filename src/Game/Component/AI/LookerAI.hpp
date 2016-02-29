#pragma once

#include <Component/SuperComponent.hpp>
#include <vector>

namespace Component {
    /// %Component providing AI which looks at the closest entity.
    class LookerAI : public SuperComponent {
        public:
            /// Create new looker AI.
            /**
             * @param entity Pointer to which entity this component corresponds.
             */
            LookerAI(Entity* entity);
            
            /// Destructor.
            ~LookerAI();

            /// How far it will look.
            float mDistance;

            /// Updates the looker AI.
            /**
             * @param deltaTime Time since last frame in seconds.
             */
            void Update(float deltaTime);

            /// Add entity to look for.
            /**
             * @param entity %Entity to look for.
             */
            void AddEntity(Entity* entity);

        private:
            // The index of the point we're targeting.
            int mTarget;

            // The points to travel through.
            std::vector<Entity*> mTargets;

    };
}
