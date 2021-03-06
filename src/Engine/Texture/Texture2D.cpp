#include "Texture2D.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_ONLY_TGA
#include <stb_image.h>

#include "../Util/Log.hpp"
#include "Default2D.vert.hpp"
#include "Texture2D.frag.hpp"
#include "../Resources.hpp"
#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Geometry/Square.hpp"
#include "../MainWindow.hpp"
#include "../Font/Font.hpp"

Texture2D::Texture2D(const char* filename, bool srgb) {
    glGenTextures(1, &mTexID);
    glBindTexture(GL_TEXTURE_2D, mTexID);
    
    // Load texture from file.
    int components;
    unsigned char* data = stbi_load(filename, &mWidth, &mHeight, &components, 0);
    
    if (data == NULL)
        Log() << "Couldn't load image " << filename << "\n";
    
    // Give the image to OpenGL.
    glTexImage2D(GL_TEXTURE_2D, 0, srgb ? GL_SRGB_ALPHA : GL_RGBA, mWidth, mHeight, 0, Format(components), GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
    
    // When MAGnifying the image (no bigger mipmap available), use LINEAR filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    // Repeat texture when texture coordinates outside 0.0-1.0.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // Generate mipmaps, by the way.
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // For rendering.
    mSquare = Resources().CreateSquare();
    
    mVertexShader = Resources().CreateShader(DEFAULT2D_VERT, DEFAULT2D_VERT_LENGTH, GL_VERTEX_SHADER);
    mFragmentShader = Resources().CreateShader(TEXTURE2D_FRAG, TEXTURE2D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ mVertexShader, mFragmentShader });
    
    mIsFromFile = true;
}

Texture2D::Texture2D(const char *source, int sourceLength, bool srgb) {
    glGenTextures(1, &mTexID);
    glBindTexture(GL_TEXTURE_2D, mTexID);
    
    // Load texture from file.
    int components;
    unsigned char* data = stbi_load_from_memory(reinterpret_cast<const unsigned char*>(source), sourceLength, &mWidth, &mHeight, &components, 0);
    
    if (data == NULL)
        Log() << "Couldn't load headerized image.\n";
    
    // Give the image to OpenGL.
    glTexImage2D(GL_TEXTURE_2D, 0, srgb ? GL_SRGB_ALPHA : GL_RGBA, mWidth, mHeight, 0, Format(components), GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
    
    // When MAGnifying the image (no bigger mipmap available), use LINEAR filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    // Repeat texture when texture coordinates outside 0.0-1.0.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // Generate mipmaps, by the way.
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // For rendering.
    mSquare = Resources().CreateSquare();
    
    mVertexShader = Resources().CreateShader(DEFAULT2D_VERT, DEFAULT2D_VERT_LENGTH, GL_VERTEX_SHADER);
    mFragmentShader = Resources().CreateShader(TEXTURE2D_FRAG, TEXTURE2D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ mVertexShader, mFragmentShader });
    
    mIsFromFile = false;
}

Texture2D::Texture2D(Font* font, const char* text) {
    mWidth = static_cast<int>(glm::ceil(font->GetWidth(text)));
    mHeight = static_cast<int>(glm::ceil(font->GetHeight()));
    
    // Texture dimensions must be divisible by 4.
    mWidth += 4 - (mWidth % 4);
    mHeight += 4 - (mHeight % 4);
    
    // Create texture.
    glGenTextures(1, &mTexID);
    glBindTexture(GL_TEXTURE_2D, mTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Create render target.
    GLuint frameBuffer;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexID, 0);
    
    // Check if framebuffer was created correctly.
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Log() << "Framebuffer creation failed\n";
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // Render.
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
    
    glViewport(0, 0, mWidth, mHeight);
    glClear(GL_COLOR_BUFFER_BIT);
    font->RenderText(text, glm::vec2(0.f, -static_cast<float>(font->GetBaseline())), 10000.f, glm::vec2(static_cast<float>(mWidth), static_cast<float>(mHeight)), false);
    
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // Free render target.
    glDeleteFramebuffers(1, &frameBuffer);
    
    // For rendering.
    mSquare = Resources().CreateSquare();
    
    mVertexShader = Resources().CreateShader(DEFAULT2D_VERT, DEFAULT2D_VERT_LENGTH, GL_VERTEX_SHADER);
    mFragmentShader = Resources().CreateShader(TEXTURE2D_FRAG, TEXTURE2D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ mVertexShader, mFragmentShader });
    
    mIsFromFile = false;
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &mTexID);
    
    Resources().FreeShaderProgram(mShaderProgram);
    Resources().FreeShader(mVertexShader);
    Resources().FreeShader(mFragmentShader);
    
    Resources().FreeSquare();
}

GLuint Texture2D::GetTextureID() const {
    return mTexID;
}

int Texture2D::GetWidth() const {
    return mWidth;
}

int Texture2D::GetHeight() const {
    return mHeight;
}

void Texture2D::SetWrapping(GLint wrapMode) {
    glBindTexture(GL_TEXTURE_2D, mTexID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
}

void Texture2D::Render(const glm::vec2 &position, const glm::vec2 &size, float alpha) const {
    // Disable depth testing.
    GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    
    // Blending enabled.
    GLboolean blend = glIsEnabled(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    mShaderProgram->Use();
    
    // Texture unit 0 is for base images.
    glUniform1i(mShaderProgram->GetUniformLocation("baseImage"), 0);
    
    // Base image texture
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, mTexID);
    
    // Set location and size.
    glm::vec2 screenSize = MainWindow::GetInstance()->GetSize();
    glUniform2fv(mShaderProgram->GetUniformLocation("position"), 1, &(position / screenSize)[0]);
    glUniform2fv(mShaderProgram->GetUniformLocation("size"), 1, &(size / screenSize)[0]);
    glUniform1f(mShaderProgram->GetUniformLocation("alpha"), alpha);
    
    glBindVertexArray(mSquare->GetVertexArray());
    
    glDrawElements(GL_TRIANGLES, mSquare->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
    
    // Reset depth testing and blending.
    if (depthTest)
        glEnable(GL_DEPTH_TEST);
    if (!blend)
        glDisable(GL_BLEND);
}

bool Texture2D::IsFromFile() const {
    return mIsFromFile;
}
