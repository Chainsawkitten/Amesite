#include "Transform.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace Component;

Transform::Transform() {
    mPosition = glm::vec3(0.f, 0.f, 0.f);
    mScale = glm::vec3(1.f, 1.f, 1.f);

    mHorizontalAngle = 0.f;
    mVerticalAngle = 0.f;
    mTiltAngle = 0.f;
}

Transform::~Transform() {

}

glm::mat4 Transform::GetOrientation() const {
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(mHorizontalAngle), glm::vec3(0, 1, 0));
    orientation = glm::rotate(orientation, glm::radians(mVerticalAngle), glm::vec3(1, 0, 0));
    orientation = glm::rotate(orientation, glm::radians(mTiltAngle), glm::vec3(0, 0, 1));
    return orientation;
}

void Transform::Rotate(float horizontalAngle, float verticalAngle, float tiltAngle) {
    mHorizontalAngle = fmodf(horizontalAngle + mHorizontalAngle, 360.f);
    if (mHorizontalAngle < 0.f)
        mHorizontalAngle += 360.f;

    mVerticalAngle = fmodf(verticalAngle + mVerticalAngle, 360.f);
    if (mVerticalAngle < 0.f)
        mVerticalAngle += 360.f;

    mTiltAngle = fmodf(tiltAngle + mTiltAngle, 360.f);
    if (mTiltAngle < 0.f)
        mTiltAngle += 360.f;
}

void Transform::Move(const glm::vec3 &offset) {
    mPosition += offset;
}

void Transform::Move(float x, float y, float z) {
    mPosition += glm::vec3(x,y,z);
}