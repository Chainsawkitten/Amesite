#include "Game.hpp"

#include <Scene/Scene.hpp>

Game::Game() {
    mActiveScene = nullptr;
}

void Game::SetScene(Scene *scene) {
    mActiveScene = scene;
}

void Game::Update(float deltaTime) {
    mActiveScene->Update(deltaTime);
}

void Game::Free() {
    if (mActiveScene != nullptr) {
        delete mActiveScene;
        mActiveScene = nullptr;
    }
}
