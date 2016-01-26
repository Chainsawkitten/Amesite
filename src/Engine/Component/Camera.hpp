#pragma once
#include <glm\glm.hpp>


namespace component{

    class Lens {
        public:
            Lens();
            ~Lens();

            glm::vec3 GetForward() const;

            glm::vec3 GetRight() const;

            glm::vec3 GetUp() const;

            glm::mat4 View() const;

            glm::mat4 Projection() const;

            glm::mat4 Orientation() const;

            float mFieldOfView;
            float mZNear;
            float zFar;


    };

}