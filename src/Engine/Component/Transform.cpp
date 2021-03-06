#include "Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace Component;

Transform::Transform(Entity* entity) : SuperComponent(entity) {
    position = glm::vec3(0.f, 0.f, 0.f);
    scale = glm::vec3(1.f, 1.f, 1.f);

    yaw = 0.f;
    pitch = 0.f;
    roll = 0.f;

    UpdateModelMatrix();
}

Transform::~Transform() {
}

void Transform::UpdateModelMatrix() {
    worldOrientationMatrix = GetLocalOrientation();
    modelMatrix = glm::translate(glm::mat4(), position) * worldOrientationMatrix * glm::scale(glm::mat4(), scale);
}

glm::vec3 Transform::GetWorldPosition() const {
    return glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
}

glm::vec3 Transform::CalculateWorldPosition() const {
    return position;
}

glm::vec3 Transform::GetWorldScale() const {
    return scale;
}

glm::vec3 Transform::GetWorldYawPitchRoll() const {
    return glm::vec3(yaw, pitch, roll);
}

glm::vec3 Transform::GetWorldDirection() const {
    return glm::normalize(glm::vec3(GetWorldOrientation() * glm::vec4(0.f, 0.f, 1.f, 0.f)));
}

glm::mat4 Transform::GetLocalOrientation() const {
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(yaw), glm::vec3(0.f, 1.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(pitch), glm::vec3(1.f, 0.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(roll), glm::vec3(0.f, 0.f, 1.f));
    return orientation;
}

glm::mat4 Transform::GetLocalCameraOrientation() const {
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(roll), glm::vec3(0.f, 0.f, 1.f));
    orientation = glm::rotate(orientation, glm::radians(pitch), glm::vec3(1.f, 0.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(yaw), glm::vec3(0.f, 1.f, 0.f));
    return orientation;
}

glm::mat4 Transform::GetWorldOrientation() const {
    return GetLocalOrientation();
}

glm::mat4 Transform::GetWorldCameraOrientation() const {
    return GetLocalCameraOrientation();
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
