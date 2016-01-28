#include "Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace Component;

Transform::Transform() {
    position = glm::vec3(0.f, 0.f, 0.f);
    scale = glm::vec3(1.f, 1.f, 1.f);

    yaw = 0.f;
    pitch = 0.f;
    roll = 0.f;
}

Transform::~Transform() {

}

glm::mat4 Transform::GetOrientation() const {
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(yaw), glm::vec3(0, 1, 0));
    orientation = glm::rotate(orientation, glm::radians(pitch), glm::vec3(1, 0, 0));
    orientation = glm::rotate(orientation, glm::radians(roll), glm::vec3(0, 0, 1));
    return orientation;
}

void Transform::Rotate(float yaw, float pitch, float roll) {
    this->yaw = fmodf(yaw + this->yaw, 360.f);
    if (this->yaw < 0.f)
        this->yaw += 360.f;

    this->pitch = fmodf(pitch + this->pitch, 360.f);
    if (this->pitch < 0.f)
        this->pitch += 360.f;

    this->roll = fmodf(roll + this->roll, 360.f);
    if (this->roll < 0.f)
        this->roll += 360.f;
}

void Transform::Move(const glm::vec3 &offset) {
    position += offset;
}

void Transform::Move(float x, float y, float z) {
    position += glm::vec3(x, y, z);
}
