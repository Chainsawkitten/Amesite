#include "MenuScene.hpp"

#include <Resources.hpp>
#include <Font/Font.hpp>
#include <Texture/Texture2D.hpp>
#include <MainWindow.hpp>
#include <Shader/Shader.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Geometry/Plane.hpp>

#include "Default3D.vert.hpp"
#include "Text3D.frag.hpp"
#include "SingleColor3D.frag.hpp"

#include <PostProcessing/PostProcessing.hpp>
#include <PostProcessing/FXAAFilter.hpp>
#include <PostProcessing/GammaCorrectionFilter.hpp>
#include <PostProcessing/GlowFilter.hpp>
#include <PostProcessing/GlowBlurFilter.hpp>
#include "../Util/GameSettings.hpp"

#include "../Util/GameEntityFactory.hpp"
#include "../Util/MainCamera.hpp"
#include "../GameObject/Camera.hpp"
#include <Entity/Entity.hpp>
#include <Component/Transform.hpp>
#include <Component/DirectionalLight.hpp>
#include <Component/Lens.hpp>

#include "../Game.hpp"
#include "MainScene.hpp"

#include <glm/gtc/matrix_transform.hpp>

MenuScene::MenuScene() {
    // Bind scene to gameEntityCreator
    GameEntityCreator().SetScene(this);
    
    // Create main camera
    mMainCamera = GameEntityCreator().CreateCamera(glm::vec3(-3.f, 1.4f, 5.f), glm::vec3(60.f, 10.f, 0.f));
    MainCameraInstance().SetMainCamera(mMainCamera->body);
    
    GameEntityCreator().CreatePlayer1(glm::vec3(0.f, 0.f, 0.f));
    
    // Assign input
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::MOVE_X, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::MOVE_Z, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::AIM_X, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::AIM_Z, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::SHOOT, InputHandler::JOYSTICK, InputHandler::RIGHT_BUMPER);
    
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::MOVE_X, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::MOVE_Z, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::AIM_X, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::AIM_Z, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::SHOOT, InputHandler::JOYSTICK, InputHandler::RIGHT_BUMPER);
    
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::UP, InputHandler::KEYBOARD, GLFW_KEY_W);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::DOWN, InputHandler::KEYBOARD, GLFW_KEY_S);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::RIGHT, InputHandler::KEYBOARD, GLFW_KEY_D);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::LEFT, InputHandler::KEYBOARD, GLFW_KEY_A);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::SHOOT, InputHandler::MOUSE, GLFW_MOUSE_BUTTON_1);
    
    // Directional light.
    Entity* dirLight = CreateEntity();
    dirLight->AddComponent<Component::Transform>()->pitch = 90.f;
    dirLight->AddComponent<Component::DirectionalLight>();
    dirLight->GetComponent<Component::DirectionalLight>()->color = glm::vec3(0.01f, 0.01f, 0.01f);
    dirLight->GetComponent<Component::DirectionalLight>()->ambientCoefficient = 0.2f;
    
    // Initialize post-processing.
    mPostProcessing = new PostProcessing(MainWindow::GetInstance()->GetSize());
    mFxaaFilter = new FXAAFilter();
    mGammaCorrectionFilter = new GammaCorrectionFilter();
    mGlowFilter = new GlowFilter();
    mGlowBlurFilter = new GlowBlurFilter();
    
    mFont = Resources().CreateFontFromFile("Resources/ABeeZee.ttf", 50.f);
    mFont->SetColor(glm::vec3(1.f, 1.f, 1.f));
    
    // Initialize shaders.
    mPlane = Resources().CreatePlane();
    Shader* vertexShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragmentShader = Resources().CreateShader(TEXT3D_FRAG, TEXT3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mTextShaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
    Resources().FreeShader(fragmentShader);
    
    fragmentShader = Resources().CreateShader(SINGLECOLOR3D_FRAG, SINGLECOLOR3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mSelectedShaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(fragmentShader);
    
    // Define menu options.
    mMenuOptions.push_back(new MenuOption(mFont, "START GAME", glm::vec3(0.f, 1.0f, 2.3f), glm::vec3(0.f, 330.f, 0.f), 0.2f));
    mMenuOptions[0]->callback = std::bind(&MenuScene::StartGame, this);
    mMenuOptions.push_back(new MenuOption(mFont, "OPTIONS", glm::vec3(0.f, 0.8f, 2.4f), glm::vec3(0.f, 330.f, 0.f), 0.2f));
    mMenuOptions.push_back(new MenuOption(mFont, "QUIT", glm::vec3(0.f, 0.6f, 2.5f), glm::vec3(0.f, 330.f, 0.f), 0.2f));
    mMenuOptions[2]->callback = std::bind(&MenuScene::Quit, this);
    mSelected = 0;
}

MenuScene::~MenuScene() {
    delete mFxaaFilter;
    delete mGammaCorrectionFilter;
    delete mGlowFilter;
    delete mGlowBlurFilter;
    delete mPostProcessing;
    
    Resources().FreeFont(mFont);
    Resources().FreePlane();
    Resources().FreeShaderProgram(mTextShaderProgram);
    Resources().FreeShaderProgram(mSelectedShaderProgram);
    
    for (MenuOption* menuOption : mMenuOptions) {
        delete menuOption;
    }
}

void MenuScene::Update(float deltaTime) {
    // Updates model matrices for this frame.
    UpdateModelMatrices();
    
    // Update menu selection.
    int movement = Input()->Triggered(InputHandler::ANYONE, InputHandler::DOWN) - Input()->Triggered(InputHandler::ANYONE, InputHandler::UP);
    if (mSelected + movement >= 0 && mSelected + movement < mMenuOptions.size())
        mSelected += movement;
    
    // Handle pressed menu option.
    if (Input()->Triggered(InputHandler::ANYONE, InputHandler::SHOOT))
        mMenuOptions[mSelected]->callback();
    
    // Render.
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    glViewport(0, 0, screenSize.x, screenSize.y);
    mRenderSystem.Render(*this, mPostProcessing->GetRenderTarget());
    
    // Glow.
    mGlowBlurFilter->SetScreenSize(screenSize);
    int blurAmount = 5;
    for (int i=0; i<blurAmount; ++i) {
        mGlowBlurFilter->SetHorizontal(true);
        mPostProcessing->ApplyFilter(mGlowBlurFilter);
        mGlowBlurFilter->SetHorizontal(false);
        mPostProcessing->ApplyFilter(mGlowBlurFilter);
    }
    mPostProcessing->ApplyFilter(mGlowFilter);
    
    RenderSelectedMenuOption(mMenuOptions[mSelected], screenSize);
    
    // Anti-aliasing.
    if (GameSettings::GetInstance().GetBool("FXAA")) {
        mFxaaFilter->SetScreenSize(screenSize);
        mPostProcessing->ApplyFilter(mFxaaFilter);
    }
    
    // Gamma correction.
    mGammaCorrectionFilter->SetBrightness((float)GameSettings::GetInstance().GetDouble("Gamma"));
    mPostProcessing->ApplyFilter(mGammaCorrectionFilter);
    
    // Render to back buffer.
    mPostProcessing->Render();
    
    // Render menu options.
    for (MenuOption* menuOption : mMenuOptions) {
        RenderMenuOption(menuOption, screenSize);
    }
}

void MenuScene::RenderSelectedMenuOption(const MenuOption* menuOption, const glm::vec2& screenSize) {
    // Blending enabled.
    GLboolean blend = glIsEnabled(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Don't write to the depth buffer.
    GLboolean depthMask;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);
    glDepthMask(GL_FALSE);
    
    mSelectedShaderProgram->Use();
    
    Entity* camera = mMainCamera->body;
    glm::mat4 viewMat = camera->GetComponent<Component::Transform>()->worldOrientationMatrix * glm::translate(glm::mat4(), -camera->GetComponent<Component::Transform>()->GetWorldPosition());
    glm::mat4 projectionMat = camera->GetComponent<Component::Lens>()->GetProjection(screenSize);
    
    glUniformMatrix4fv(mSelectedShaderProgram->GetUniformLocation("view"), 1, GL_FALSE, &viewMat[0][0]);
    glUniformMatrix4fv(mSelectedShaderProgram->GetUniformLocation("projection"), 1, GL_FALSE, &projectionMat[0][0]);
    
    glBindVertexArray(mPlane->GetVertexArray());
    
    glm::mat4 modelMat = menuOption->GetModelMatrix();
    glUniformMatrix4fv(mSelectedShaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &modelMat[0][0]);
    glm::mat4 normalMat = glm::transpose(glm::inverse(viewMat * modelMat));
    glUniformMatrix3fv(mSelectedShaderProgram->GetUniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(normalMat)[0][0]);
    
    glUniform4fv(mSelectedShaderProgram->GetUniformLocation("color"), 1, &glm::vec4(0.f, 0.f, 0.f, 0.65f)[0]);
    
    glDrawElements(GL_TRIANGLES, mPlane->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
    
    glUseProgram(0);
    
    // Reset depth and blending.
    if (!blend)
        glDisable(GL_BLEND);
    if (depthMask)
        glDepthMask(GL_TRUE);
}

void MenuScene::RenderMenuOption(const MenuOption* menuOption, const glm::vec2& screenSize) {
    // Blending enabled.
    GLboolean blend = glIsEnabled(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    mTextShaderProgram->Use();
    
    Entity* camera = mMainCamera->body;
    glm::mat4 viewMat = camera->GetComponent<Component::Transform>()->worldOrientationMatrix * glm::translate(glm::mat4(), -camera->GetComponent<Component::Transform>()->GetWorldPosition());
    glm::mat4 projectionMat = camera->GetComponent<Component::Lens>()->GetProjection(screenSize);
    
    glUniformMatrix4fv(mTextShaderProgram->GetUniformLocation("view"), 1, GL_FALSE, &viewMat[0][0]);
    glUniformMatrix4fv(mTextShaderProgram->GetUniformLocation("projection"), 1, GL_FALSE, &projectionMat[0][0]);
    
    glBindVertexArray(mPlane->GetVertexArray());
    
    // Texture.
    glUniform1i(mTextShaderProgram->GetUniformLocation("baseImage"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, menuOption->prerenderedText->GetTextureID());
    
    glm::mat4 modelMat = menuOption->GetModelMatrix();
    glUniformMatrix4fv(mTextShaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &modelMat[0][0]);
    glm::mat4 normalMat = glm::transpose(glm::inverse(viewMat * modelMat));
    glUniformMatrix3fv(mTextShaderProgram->GetUniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(normalMat)[0][0]);
    
    glDrawElements(GL_TRIANGLES, mPlane->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
    
    glUseProgram(0);
    
    // Reset blending.
    if (!blend)
        glDisable(GL_BLEND);
}

MenuScene::MenuOption::MenuOption(Font* font, const char* text, const glm::vec3& position, const glm::vec3& rotation, float height) {
    prerenderedText = new Texture2D(font, text);
    this->position = position;
    this->rotation = rotation;
    scale = glm::vec2(height * static_cast<float>(prerenderedText->GetWidth()) / static_cast<float>(prerenderedText->GetHeight()), height);
    callback = std::bind(&MenuScene::MenuOption::EmptyCallback, this);
}

MenuScene::MenuOption::~MenuOption() {
    delete prerenderedText;
}

glm::mat4 MenuScene::MenuOption::GetModelMatrix() const {
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(rotation.x), glm::vec3(0.f, 1.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(rotation.y), glm::vec3(1.f, 0.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
    
    return glm::translate(glm::mat4(), position) * orientation * glm::scale(glm::mat4(), glm::vec3(scale.x, scale.y, 1.f));
}

void MenuScene::MenuOption::EmptyCallback() const {
    
}

void MenuScene::StartGame() {
    Game::GetInstance().SetScene(new MainScene());
}

void MenuScene::Quit() {
    MainWindow::GetInstance()->Close();
}
