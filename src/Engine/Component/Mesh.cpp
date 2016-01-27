#include "Mesh.hpp"

using namespace Component;

Mesh::Mesh() {
    mGeometry = nullptr;
}

Mesh::~Mesh() {
    if (mGeometry != nullptr)
        delete mGeometry;
}