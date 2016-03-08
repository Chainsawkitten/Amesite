#include "LoadingScene.hpp"

#include "../Game.hpp"
#include "MenuScene.hpp"

LoadingScene::LoadingScene() {
    
}

LoadingScene::~LoadingScene() {
    
}

void LoadingScene::Update(float deltaTime) {
    Game::GetInstance().SetScene(new MenuScene());
}
