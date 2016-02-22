#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>

class Shader;
class ShaderProgram;
namespace Geometry {
    class Map;
    class Cube;
    class Square;
    class Model;
    class OBJModel;
}
class Texture2D;
namespace Audio {
    class SoundBuffer;
}
class Font;

/// Handles all resource loading.
class ResourceManager {
    public:
        /// Get the instance of the class.
        /**
         * @return The %ResourceManager instance
         */
        static ResourceManager& GetInstance();
        
        /// Create a shader if it doesn't already exist.
        /**
         * @param source GLSL code for the shader.
         * @param sourceLength Length of the GLSL source code.
         * @param shaderType %Shader type. One of GL_COMPUTE_SHADER, GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or GL_FRAGMENT_SHADER.
         * @return The shader instance
         */
        Shader* CreateShader(const char* source, int sourceLength, GLenum shaderType);
        
        /// Free the reference to the shader.
        /**
         * Deletes the instance if no more references exist.
         * @param shader %Shader to dereference.
         */
        void FreeShader(Shader* shader);
        
        /// Create shader program if it doesn't already exist.
        /**
         * Link together shaders into a shader program that can be run on the GPU.
         *
         * Sample:
         * \code{.cpp}
         * Shader* vertexShader = new Shader(vertexSource, vertexSourceLength, GL_VERTEX_SHADER);
         * Shader* geometryShader = new Shader(geometrySource, geometrySourceLength, GL_GEOMETRY_SHADER);
         * Shader* fragmentShader = new Shader(fragmentSource, fragmentSourceLength, GL_FRAGMENT_SHADER);
         * ShaderProgram* shaderProgram = new ResourceManager::GetInstance().CreateShaderProgram({ vertexShader, geometryShader, fragmentShader });
         * \endcode
         *
         * @param shaders List of shaders to link together.
         * @return The shader program instance.
         */
        ShaderProgram* CreateShaderProgram(std::initializer_list<const Shader*> shaders);
        
        /// Free the reference to a shader program.
        /**
         * Deletes the instance if no more references exist.
         * @param shaderProgram %Shader program to dereference.
         */
        void FreeShaderProgram(ShaderProgram* shaderProgram);

        /// Create a map for rendering if it doesn't already exist.
        /**
        * @return The map instance.
        */
        Geometry::Map* CreateMap(bool **data, const float squareSize, glm::uvec2 dataDimensions);

        /// Free the reference to the map.
        /**
        * Deletes the instance if no more references exist.
        */
        void FreeMap();
        
        /// Create a cube for rendering if it doesn't already exist.
        /**
         * @return The cube instance.
         */
        Geometry::Cube* CreateCube();
        
        /// Free the reference to the cube.
        /**
         * Deletes the instance if no more references exist.
         */
        void FreeCube();
        
        /// Create a square for rendering if it doesn't already exist.
        /**
         * @return The square instance.
         */
        Geometry::Square* CreateSquare();
        
        /// Free the reference to the square.
        /**
         * Deletes the instance if no more references exist.
         */
        void FreeSquare();
        
        /// Create a 2D texture if it doesn't already exist.
        /**
         * @param data Image file data.
         * @param dataLength Length of the image file data.
         * @param srgb Whether the image is in SRGB space and should be converted to linear space.
         * @return The %Texture2D instance.
         */
        Texture2D* CreateTexture2D(const char* data, int dataLength, bool srgb = false);
        
        /// Create a 2D texture if it doesn't already exist.
        /**
         * @param filename Filename of image file.
         * @param srgb Whether the image is in SRGB space and should be converted to linear space.
         * @return The %Texture2D instance.
         */
        Texture2D* CreateTexture2DFromFile(std::string filename, bool srgb = false);

        /// Create an OBJ model for rendering if it doesn't already exist.
        /**
        * @param filename Filename of model file.
        * @return The model instance
        */
        Geometry::OBJModel* CreateOBJModel(std::string filename);

        /// Free the reference to the model.
        /**
        * @param model %Model to dereference.
        */
        void FreeOBJModel(Geometry::OBJModel* model);
        
        /// Free the reference to the 2D texture.
        /**
         * Deletes the instance if no more references exist.
         * @param texture %Texture to dereference.
         */
        void FreeTexture2D(Texture2D* texture);
        
        /// Create a sound if it doesn't already exist.
        /**
         * Supported formats: 16-bit PCM Wave, Ogg Vorbis.
         * @param filename Path to the sound file.
         * @return The %SoundBuffer instance.
         */
        Audio::SoundBuffer* CreateSound(std::string filename);
        
        /// Free the reference to the sound.
        /**
         * Deletes the instance if no more references exist.
         * @param soundBuffer %SoundBuffer to dereference.
         */
        void FreeSound(Audio::SoundBuffer* soundBuffer);
        
        /// Create a font if it doesn't already exist.
        /**
         * @param source TTF source.
         * @param sourceLength Length of the source.
         * @param height Character height.
         * @return The %Font instance
         */
        Font* CreateFontEmbedded(const char* source, int sourceLength, float height);
        
        /// Create a font if it doesn't already exist.
        /**
         * @param filename Filename of the TTF file.
         * @param height Character height.
         * @return The %Font instance
         */
        Font* CreateFontFromFile(std::string filename, float height);
        
        /// Free the reference to the font.
        /**
         * Deletes the instance if no more references exist.
         * @param font %Font to dereference.
         */
        void FreeFont(Font* font);
        
    private:
        ResourceManager();
        ResourceManager(ResourceManager const&);
        void operator=(ResourceManager const&);
        
        // Shaders
        struct ShaderInstance {
            Shader* shader;
            int count;
        };
        std::map<const char*, ShaderInstance> mShaders;
        std::map<Shader*, const char*> mShadersInverse;
        
        // ShaderPrograms
        struct ShaderProgramInstance {
            ShaderProgram* shaderProgram;
            int count;
        };
        struct ShaderProgramKey {
            const char* computeShader;
            const char* vertexShader;
            const char* tessControlShader;
            const char* tessEvaluationShader;
            const char* geometryShader;
            const char* fragmentShader;
            
            ShaderProgramKey();
            
            bool operator<(const ShaderProgramKey& other) const;
        };
        std::map<ShaderProgramKey, ShaderProgramInstance> mShaderPrograms;
        std::map<ShaderProgram*, ShaderProgramKey> mShaderProgramsInverse;

        // Map
        Geometry::Map* mMap;
        int mMapCount;
        
        // Cube
        Geometry::Cube* mCube;
        int mCubeCount;
        
        // Square
        Geometry::Square* mSquare;
        int mSquareCount;
        
        // Texture2D
        struct Texture2DInstance {
            Texture2D* texture;
            int count;
        };
        std::map<const char*, Texture2DInstance> mTextures;
        std::map<Texture2D*, const char*> mTexturesInverse;

        // OBJ Model
        struct OBJModelInstance {
            Geometry::OBJModel* model;
            int count;
        };
        std::map<std::string, OBJModelInstance> objModels;
        std::map<Geometry::OBJModel*, std::string> objModelsInverse;
        
        // Texture2D from file
        std::map<std::string, Texture2DInstance> mTexturesFromFile;
        std::map<Texture2D*, std::string> mTexturesFromFileInverse;
        
        // Sound
        struct SoundInstance {
            Audio::SoundBuffer* soundBuffer;
            int count;
        };
        std::map<std::string, SoundInstance> mSounds;
        std::map<Audio::SoundBuffer*, std::string> mSoundsInverse;
        
        // Font
        struct FontInstance {
            Font* font;
            int count;
        };
        struct FontKey {
            const char* source;
            float height;
            
            FontKey();
            
            bool operator<(const FontKey& other) const;
        };
        std::map<FontKey, FontInstance> mFonts;
        std::map<Font*, FontKey> mFontsInverse;
        
        // Font from file
        struct FontFromFileKey {
            std::string filename;
            float height;
            
            FontFromFileKey();
            
            bool operator<(const FontFromFileKey& other) const;
        };
        std::map<FontFromFileKey, FontInstance> mFontsFromFile;
        std::map<Font*, FontFromFileKey> mFontsFromFileInverse;
};

/// Get the resource manager.
/**
 * @return The %ResourceManager instance
 */
ResourceManager& Resources();
