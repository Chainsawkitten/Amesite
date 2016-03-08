#include "LoadingScene.hpp"

#include "../Game.hpp"
#include "MenuScene.hpp"

LoadingScene::LoadingScene() {
    mShown = false;
}

LoadingScene::~LoadingScene() {
    
}

void LoadingScene::Update(float deltaTime) {
    if (mShown)
        Game::GetInstance().SetScene(new MenuScene());
    
    mShown = true;
}
