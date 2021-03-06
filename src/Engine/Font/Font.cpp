#include "Font.hpp"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <stdio.h>
#include <fstream>
#include "../Util/Log.hpp"
#include "../Resources.hpp"
#include "Font.vert.hpp"
#include "Font.frag.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Geometry/Square.hpp"
#include "../MainWindow.hpp"

Font::Font(const char* filename, float height) {
    mHeight = height;
    
    // Read font file into buffer.
    std::ifstream fontFile;
    fontFile.open(filename, std::ios_base::binary | std::ios_base::ate);
    if (!fontFile.is_open())
        Log() << "Couldn't open font file " << filename << " for reading.\n";
    
    unsigned int size = static_cast<unsigned int>(fontFile.tellg());
    unsigned char* ttfBuffer = new unsigned char[size];
    fontFile.seekg(0, std::ios::beg);
    fontFile.read(reinterpret_cast<char*>(ttfBuffer), size);
    fontFile.close();
    
    mBitmapWidth = static_cast<unsigned int>((ceil(sqrt(96.f)) + 1.f) * height + 0.05f);
    // Textures must be divisible by 4.
    mBitmapWidth += 4 - (mBitmapWidth % 4);
    unsigned char* tempBitmap = new unsigned char[mBitmapWidth * mBitmapWidth];
    
    stbtt_BakeFontBitmap(ttfBuffer, 0, height, tempBitmap, mBitmapWidth, mBitmapWidth, 32, 96, mCData);
    
    // Get baseline.
    stbtt_fontinfo fontInfo;
    stbtt_InitFont(&fontInfo, ttfBuffer, 0);
    float scale = stbtt_ScaleForPixelHeight(&fontInfo, mHeight);
    int ascent;
    stbtt_GetFontVMetrics(&fontInfo, &ascent, 0, 0);
    mBaseline = static_cast<int>(mHeight - ascent * scale);
    
    delete[] ttfBuffer;
    
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, mBitmapWidth, mBitmapWidth, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tempBitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    delete[] tempBitmap;
    
    // Resources.
    mSquare = Resources().CreateSquare();
    
    mVertexShader = Resources().CreateShader(FONT_VERT, FONT_VERT_LENGTH, GL_VERTEX_SHADER);
    mFragmentShader = Resources().CreateShader(FONT_FRAG, FONT_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ mVertexShader, mFragmentShader });
    
    mColor = glm::vec3(0.f, 0.f, 0.f);
    mIsFromFile = true;
}

Font::Font(const char* source, int sourceLength, float height) {
    mHeight = height;
    
    mBitmapWidth = static_cast<unsigned int>((ceil(sqrt(96.f)) + 1.f) * height + 0.05f);
    mBitmapWidth += mBitmapWidth % 4;
    unsigned char* tempBitmap = new unsigned char[mBitmapWidth * mBitmapWidth];
    
    stbtt_BakeFontBitmap(reinterpret_cast<const unsigned char*>(source), 0, height, tempBitmap, mBitmapWidth, mBitmapWidth, 32, 96, mCData);
    
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, mBitmapWidth, mBitmapWidth, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tempBitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    delete[] tempBitmap;
    
    // Resources.
    mSquare = Resources().CreateSquare();
    
    mVertexShader = Resources().CreateShader(FONT_VERT, FONT_VERT_LENGTH, GL_VERTEX_SHADER);
    mFragmentShader = Resources().CreateShader(FONT_FRAG, FONT_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ mVertexShader, mFragmentShader });
    
    mColor = glm::vec3(0.f, 0.f, 0.f);
    mIsFromFile = false;
}

Font::~Font() {
    glDeleteTextures(1, &mTexture);
    
    Resources().FreeShader(mVertexShader);
    Resources().FreeShader(mFragmentShader);
    Resources().FreeShaderProgram(mShaderProgram);
    
    Resources().FreeSquare();
}

void Font::RenderText(const char* text, const glm::vec2& position, float wrap, glm::vec2 screenSize, bool blending) {
    if (screenSize == glm::vec2(0.f, 0.f))
        screenSize = MainWindow::GetInstance()->GetSize();
    
    // Disable depth testing
    GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    
    mShaderProgram->Use();
    
    // Blending enabled
    GLboolean blend = glIsEnabled(GL_BLEND);
    if (blending) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    
    glBindVertexArray(mSquare->GetVertexArray());
    
    // Texture unit 0 is for base images.
    glUniform1i(mShaderProgram->GetUniformLocation("baseImage"), 0);
    
    // Base image texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    
    glm::vec2 pos = position;
    pos.y += mHeight;
    while (*text) {
        if (*text >= 32 && *text < 128)
            pos.x = RenderCharacter(*text, pos, screenSize);
        
        if (pos.x > position.x + wrap || *text == '\n') {
            pos.x = position.x;
            pos.y += mHeight;
        }
        ++text;
    }
    
    if (depthTest)
        glEnable(GL_DEPTH_TEST);
    if (!blend)
        glDisable(GL_BLEND);
}

glm::vec3 Font::GetColor() const {
    return mColor;
}

void Font::SetColor(const glm::vec3& color) {
    mColor = color;
}

float Font::GetWidth(const char* text) {
    float x = 0.f;
    float y = 0.f;
    
    stbtt_aligned_quad q;
    while (*text) {
        q = BakedQuad(*text, x, y);
        ++text;
    }
    
    return q.x1;
}

float Font::GetHeight() const {
    return mHeight;
}

int Font::GetBaseline() const {
    return mBaseline;
}

bool Font::IsFromFile() const {
    return mIsFromFile;
}

float Font::RenderCharacter(char character, const glm::vec2& position, const glm::vec2& screenSize) {
    stbtt_aligned_quad q;
    float x = position.x;
    float y = position.y;
    q = BakedQuad(character, x, y);
    
    glm::vec2 pos = glm::vec2(q.x0, q.y0);
    glm::vec2 siz = glm::vec2(q.x1, q.y1) - glm::vec2(q.x0, q.y0);
    
    glUniform2fv(mShaderProgram->GetUniformLocation("position"), 1, &(pos / screenSize)[0]);
    glUniform2fv(mShaderProgram->GetUniformLocation("size"), 1, &(siz / screenSize)[0]);
    
    glm::vec2 uv1 = glm::vec2(q.s0, q.t0);
    glm::vec2 uv2 = glm::vec2(q.s1, q.t1);
    glUniform2fv(mShaderProgram->GetUniformLocation("uv1"), 1, &uv1[0]);
    glUniform2fv(mShaderProgram->GetUniformLocation("uv2"), 1, &uv2[0]);
    
    glUniform3fv(mShaderProgram->GetUniformLocation("color"), 1, &mColor[0]);
    
    glDrawElements(GL_TRIANGLES, mSquare->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
    
    return x;
}

stbtt_aligned_quad Font::BakedQuad(char character, float& x, float& y) {
    stbtt_aligned_quad q;
    stbtt_GetBakedQuad(mCData, mBitmapWidth, mBitmapWidth, character - 32, &x, &y, &q, 1);
    return q;
}
