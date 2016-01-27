#include "Mesh.hpp"

using namespace Component;

Mesh::Mesh() {
    geometry = nullptr;
}

Mesh::~Mesh() {
    if (geometry != nullptr)
        delete geometry;
}