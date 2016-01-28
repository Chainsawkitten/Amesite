#include "Parent.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

using namespace Component;

Parent::Parent() {
    //fieldOfView = 45.f;
    //zFar = 100.f;
    //zNear = 0.05f;
}

Parent::~Parent() {

}

//glm::mat4 Lens::GetProjection(const glm::vec2& screenSize) const {
//    return glm::perspective(glm::radians(fieldOfView), screenSize.x/screenSize.y, zNear, zFar);
//}
