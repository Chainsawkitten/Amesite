#pragma once
#include <glm\glm.hpp>

namespace Component {

    ///Component handling camera
    class Lens {
        public:
            ///Create a new lens
            Lens();
            ///Destructor
            ~Lens();

            ///Create projection matrix
            /**
            *@param screenSize
            *@return mat4 containing projection matrix
            */
            glm::mat4 Projection(const glm::vec2& screenSize) const;

            float mFieldOfView;
            float mZNear;
            float mZFar;
    };

}