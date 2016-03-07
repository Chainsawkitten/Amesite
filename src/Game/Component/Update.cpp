#include "Update.hpp"

using namespace Component;

Update::Update(Entity* entity) : SuperComponent(entity) {
    updateFunction = std::bind(&Update::mEmptyFunction, this);
}

Update::~Update() {
}

void Update::mEmptyFunction() {
}
