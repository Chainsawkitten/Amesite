set(ENGINE_SRCS
    MainWindow.cpp
    RenderTarget.cpp
    Resources.cpp
    Audio/SoundBuffer.cpp
    Audio/VorbisFile.cpp
    Audio/WaveFile.cpp
    Font/Font.cpp
    Geometry/Geometry2D.cpp
    Geometry/Square.cpp
    Geometry/Geometry3D.cpp
    Geometry/Terrain.cpp
    Geometry/Cube.cpp
    Geometry/Plane.cpp
    Geometry/OBJModel.cpp
    Geometry/Map.cpp
    Lighting/DeferredLighting.cpp
    Shader/Shader.cpp
    Shader/ShaderProgram.cpp
    Util/FileSystem.cpp
    Util/Log.cpp
    Util/Settings.cpp
    Util/Input.cpp
    Util/Picking.cpp
    Component/SuperComponent.cpp
    Component/Lens.cpp
    Component/Transform.cpp
    Component/Mesh.cpp
    Component/Material.cpp
    Component/RelativeTransform.cpp
    Component/Physics.cpp
    Component/Collider2DRectangle.cpp
    Component/Collider2DCircle.cpp
    Component/ParticleEmitter.cpp
    Component/DirectionalLight.cpp
    Component/PointLight.cpp
    Component/SpotLight.cpp
    Component/Animation.cpp
    Component/SoundSource.cpp
    Component/Listener.cpp
    Entity/Entity.cpp
    Scene/Scene.cpp
    System/RenderSystem.cpp
    System/PhysicsSystem.cpp
    System/CollisionSystem.cpp
    System/ParticleSystem.cpp
    System/ParticleRenderSystem.cpp
    System/AnimationSystem.cpp
    System/SoundSystem.cpp
    System/DebugDrawingSystem.cpp
    Texture/Texture.cpp
    Texture/Texture2D.cpp
    PostProcessing/PostProcessing.cpp
    PostProcessing/ColorFilter.cpp
    PostProcessing/FXAAFilter.cpp
    PostProcessing/FogFilter.cpp
    PostProcessing/GammaCorrectionFilter.cpp
    PostProcessing/GlowFilter.cpp
    PostProcessing/GlowBlurFilter.cpp
    GameObject/SuperGameObject.cpp
    Physics/AxisAlignedBoundingBox.cpp
    Physics/Frustum.cpp
    Physics/Sphere.cpp
    Profiling/ProfilingManager.cpp
    Profiling/CPUProfiling.cpp
    Profiling/GPUProfiling.cpp
    Profiling/Query.cpp
)

set(ENGINE_HEADERS
    MainWindow.hpp
    RenderTarget.hpp
    Resources.hpp
    Audio/SoundBuffer.hpp
    Audio/SoundFile.hpp
    Audio/VorbisFile.hpp
    Audio/WaveFile.hpp
    Font/Font.hpp
    Geometry/Geometry2D.hpp
    Geometry/Square.hpp
    Geometry/Geometry3D.hpp
    Geometry/Terrain.hpp
    Geometry/Cube.hpp
    Geometry/Plane.hpp
    Geometry/OBJModel.hpp
    Geometry/Map.hpp
    Lighting/DeferredLighting.hpp
    Shader/Shader.hpp
    Shader/ShaderProgram.hpp
    Util/FileSystem.hpp
    Util/Log.hpp
    Util/Settings.hpp
    Util/Input.hpp
    Util/Picking.hpp
    Component/SuperComponent.hpp
    Component/Lens.hpp
    Component/Transform.hpp
    Component/Mesh.hpp
    Component/Material.hpp
    Component/RelativeTransform.hpp
    Component/Physics.hpp
    Component/Collider2DRectangle.hpp
    Component/Collider2DCircle.hpp
    Component/ParticleEmitter.hpp
    Component/DirectionalLight.hpp
    Component/PointLight.hpp
    Component/SpotLight.hpp
    Component/Animation.hpp
    Component/SoundSource.hpp
    Component/Listener.hpp
    Entity/Entity.hpp
    Scene/Scene.hpp
    System/RenderSystem.hpp
    System/PhysicsSystem.hpp
    System/CollisionSystem.hpp
    System/ParticleSystem.hpp
    System/ParticleRenderSystem.hpp
    System/AnimationSystem.hpp
    System/SoundSystem.hpp
    System/DebugDrawingSystem.hpp
    Texture/Texture.hpp
    Texture/Texture2D.hpp
    PostProcessing/PostProcessing.hpp
    PostProcessing/Filter.hpp
    PostProcessing/ColorFilter.hpp
    PostProcessing/FXAAFilter.hpp
    PostProcessing/FogFilter.hpp
    PostProcessing/GammaCorrectionFilter.hpp
    PostProcessing/GlowFilter.hpp
    PostProcessing/GlowBlurFilter.hpp
    GameObject/SuperGameObject.hpp
    Physics/AxisAlignedBoundingBox.hpp
    Physics/Frustum.hpp
    Physics/Sphere.hpp
    Profiling/ProfilingManager.hpp
    Profiling/CPUProfiling.hpp
    Profiling/GPUProfiling.hpp
    Profiling/Query.hpp
)

# Setup embedded source
set(EMBEDDED_INPUT
    ../../shaders/Default3D.frag
    ../../shaders/Default3D.vert
    ../../shaders/Default2D.vert
    ../../shaders/DebugDrawing.vert
    ../../shaders/DebugDrawing.frag
    ../../shaders/SingleColor2D.frag
    ../../shaders/Texture2D.frag
    ../../shaders/Font.vert
    ../../shaders/Font.frag
    ../../shaders/Text3D.frag
    ../../shaders/SingleColor3D.frag
    ../../shaders/Post.vert
    ../../shaders/PostCopy.frag
    ../../shaders/PostColor.frag
    ../../shaders/PostDither.frag
    ../../shaders/PostFXAA.frag
    ../../shaders/PostFog.frag
    ../../shaders/PostGamma.frag
    ../../shaders/PostGlow.frag
    ../../shaders/PostGlowBlur.frag
    ../../shaders/Deferred.frag
    ../../shaders/Particle.vert
    ../../shaders/Particle.geom
    ../../shaders/Particle.frag
)

foreach(item ${EMBEDDED_INPUT})
    ProcessWithHeaderize(EMBEDDED_HEADER EMBEDDED_SOURCE ${item} ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

create_directory_groups(${ENGINE_SRCS} ${ENGINE_HEADERS})

add_library(Engine STATIC ${ENGINE_SRCS} ${ENGINE_HEADERS} ${EMBEDDED_SOURCE})
target_link_libraries(Engine glew glfw ${GLFW_LIBRARIES} glm OpenALSoft simpleini stb)
set_property(TARGET Engine PROPERTY CXX_STANDARD 11)
set_property(TARGET Engine PROPERTY CXX_STANDARD_REQUIRED ON)
