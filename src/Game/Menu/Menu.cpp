#include "Menu.hpp"

#include <Resources.hpp>
#include <Font/Font.hpp>
#include <MainWindow.hpp>
#include <Shader/Shader.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Geometry/Plane.hpp>

#include "Default3D.vert.hpp"
#include "Text3D.frag.hpp"
#include "SingleColor3D.frag.hpp"

Menu::Menu() {
    // Load font.
    float fontHeight = glm::ceil(MainWindow::GetInstance()->GetSize().y * 0.07f);
    mFont = Resources().CreateFontFromFile("Resources/ABeeZee.ttf", fontHeight);
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
}

Menu::~Menu() {
    Resources().FreeFont(mFont);
    Resources().FreePlane();
    Resources().FreeShaderProgram(mTextShaderProgram);
    Resources().FreeShaderProgram(mSelectedShaderProgram);
}

bool Menu::IsActive() const {
    return true;
}

void Menu::Update() {
    /// @todo
}

void Menu::RenderSelected() {
    /// @todo
}

void Menu::RenderMenuOptions() {
    /// @todo
}
