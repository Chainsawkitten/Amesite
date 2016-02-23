#include "Game.hpp"

#include <Scene/Scene.hpp>

Game::Game() {
    mActiveScene = nullptr;
    mNextScene = nullptr;
}

void Game::SetScene(Scene *scene) {
    if (mNextScene != nullptr)
        delete mNextScene;
    mNextScene = scene;
}

void Game::Update(float deltaTime) {
    if (mNextScene != nullptr) {
        delete mActiveScene;
        mActiveScene = mNextScene;
        mNextScene = nullptr;
    }
    
    mActiveScene->Update(deltaTime);
}

void Game::Free() {
    if (mActiveScene != nullptr) {
        delete mActiveScene;
        mActiveScene = nullptr;
    }
    if (mNextScene != nullptr) {
        delete mNextScene;
        mNextScene = nullptr;
    }
}
