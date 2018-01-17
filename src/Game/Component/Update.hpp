#pragma once

#include <Engine/Component/SuperComponent.hpp>
#include <functional>

namespace Component {
    /// %Component containing en update fucntion.
    class Update : public SuperComponent {
        public:
            /// Create Update component.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Update(Entity* entity);
            
            /// Destructor.
            ~Update();

            /// Update function.
            /**
             * Default: EmptyFunction
             */
            std::function<void()> updateFunction;

        private:
            /// Empty function.
            void mEmptyFunction();
    };
}
