cmake_minimum_required(VERSION 2.8.12)

project(Modership)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "-g -gdwarf-2 -static-libstdc++")
endif()

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Don't build GLFW examples" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Don't build GLFW tests" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Don't build GLFW docs" FORCE)
add_subdirectory(externals/glfw)
include_directories(externals/glfw/include)

# GLEW
add_subdirectory(externals/glew/build/cmake)
include_directories(externals/glew/include)

# SimpleIni
include_directories(externals/simpleini)

# GLM
include_directories(externals/glm)

# STB
include_directories(externals/stb)

# OpenAL Soft
set(ALSOFT_EXAMPLES OFF CACHE BOOL "Don't build OpenAL Soft examples" FORCE)
add_subdirectory(externals/openal-soft)
include_directories(externals/openal-soft/include)
if(WIN32)
    set(OPENAL_LIB OpenAL32)
else()
    set(OPENAL_LIB openal)
endif()

# This function should be passed a list of all files in a target. It will automatically generate
# file groups following the directory hierarchy, so that the layout of the files in IDEs matches the
# one in the filesystem.
function(create_directory_groups)
    # Place any files that aren't in the source list in a separate group so that they don't get in
    # the way.
    source_group("Other Files" REGULAR_EXPRESSION ".")

    foreach(file_name ${ARGV})
        get_filename_component(dir_name "${file_name}" PATH)
        # Group names use '\' as a separator even though the entire rest of CMake uses '/'...
        string(REPLACE "/" "\\" group_name "${dir_name}")
        source_group("${group_name}" FILES "${file_name}")
    endforeach()
endfunction()

# Coding style
find_program(VERA vera++
    DOC "Location of vera++"
)
if(NOT ${VERA} MATCHES "VERA-NOTFOUND")
    file(GLOB_RECURSE VERA_SRCS
            ${CMAKE_SOURCE_DIR}/src/*.cpp
            ${CMAKE_SOURCE_DIR}/src/*.h
        )
    add_custom_target(VeraStyle ALL
            ${VERA}
            -r ${CMAKE_SOURCE_DIR}/vera
            -p modership
            --warning
            ${VERA_SRCS}
        )
endif()

# Copy resources
set(RESOURCES
        assets/ABeeZee.ttf
        assets/Mugglorna.png
        assets/TestTexture.png
        assets/DefaultDiffuse.png
        assets/DefaultNormal.png
        assets/DefaultSpecular.png
        assets/DefaultGlow.png
        assets/ParticleAtlas.png
        assets/Normal.png
        assets/Height.png
        assets/Glow.png
        assets/Albedo.png
        assets/enemy_head_crystal_diff.png
        assets/enemy_head_crystal_spec.png
        assets/enemy_head_crystal_glow.png
        assets/enemy_tail_crystal_diff.png
        assets/enemy_tail_crystal_spec.png
        assets/enemy_tail_crystal_glow.png
        assets/enemy_head_crystal.obj
        assets/enemy_tail_crystal.obj
        assets/player1/player1_body_diff_healthy.png
        assets/player1/player1_body_diff_heavy_damage.png
        assets/player1/player1_body_diff_medium_damage.png
        assets/player1/player1_body_spec.png
        assets/player1/player1_body_glow.png
        assets/player1/player1_engine_diff.png
        assets/player1/player1_body.obj
        assets/player1/player1_frontEngine.obj
        assets/player1/player1_backEngine.obj
        assets/player1/player1_turret_barrel.obj
        assets/player1/player1_turret_body.obj
        assets/player2/player2_diff.png
        assets/player2/player2_diff_medium_damage.png
        assets/player2/player2_diff_heavy_damage.png
        assets/player2/player2_glow.png
        assets/player2/player2_norm.png
        assets/player2/player2_spec.png
        assets/player2/player2_body.obj
        assets/player2/player2_engine.obj
        assets/player2/player2_propeller.obj
        assets/pylon/Pylon_01_spec.png
        assets/pylon/Pylon_01_glow.png
        assets/pylon/Pylon_01_diff.png
        assets/pylon/Pylon_01.obj
        assets/crystal/Crystal_01_spec.png
        assets/crystal/Crystal_01_glow.png
        assets/crystal/Crystal_01_diff.png
        assets/crystal/Crystal_01.obj
        assets/shield/Shield_01_diff.png
        assets/shield/Shield_01_glow.png
        assets/shield/Shield_01_spec.png
        assets/shield/Shield_01.obj
        assets/circle/Circle.obj
        assets/circle/Circle_red_diff.png
        assets/circle/Circle_green_diff.png
        assets/circle/Circle_blue_diff.png
        assets/spawn/altar/altar_diff.png
        assets/spawn/altar/altar_glow.png
        assets/spawn/altar/altar.obj
        assets/spawn/pillar/pillar_diff.png
        assets/spawn/pillar/pillar_glow.png
        assets/spawn/pillar/pillar.obj
        assets/spawn/portal/portal.obj
        assets/diamond/diamond_body.obj
        assets/nest/nest_body.obj
        assets/ring/ring_body.obj
        assets/ringSegment/ring_segment.obj
        assets/rocket/rocket_body.obj
        assets/wall_diff.png
        assets/wall_norm.png
        assets/wall2_diff.png
        assets/wall2_norm.png
        assets/wall2_spec.png
        assets/wall_gray.png
        assets/ground_sand.png
        assets/GolfBall.obj
        assets/MusicAction.ogg
        assets/MusicCalm.ogg
        assets/Laser.ogg
        assets/SwirlingPortal.png
        assets/Explosion01.ogg
        assets/Explosion02.ogg
        assets/Explosion03.ogg
        assets/Explosion04.ogg
        assets/Explosion05.ogg
    )

add_custom_target(ResourceTarget ALL)

function(CopyAsset inputfile outdir)
    get_filename_component(basefile ${inputfile} NAME)
    set(outputfile ${outdir}/${basefile})
    
    add_custom_command(
        TARGET ResourceTarget
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${inputfile} ${outputfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Copying ${inputfile}"
    )
endfunction()

foreach(item ${RESOURCES})
    CopyAsset(${item} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Resources)
endforeach()

# Setup embedded source
set(EMBEDDED_INPUT
        shaders/Default3D.frag
        shaders/Default3D.vert
        shaders/Default2D.vert
        shaders/SingleColor2D.frag
        shaders/Texture2D.frag
        shaders/Font.vert
        shaders/Font.frag
        shaders/Text3D.frag
        shaders/SingleColor3D.frag
        shaders/Post.vert
        shaders/PostCopy.frag
        shaders/PostColor.frag
        shaders/PostFXAA.frag
        shaders/PostFog.frag
        shaders/PostGamma.frag
        shaders/PostGlow.frag
        shaders/PostGlowBlur.frag
        shaders/Deferred.frag
        shaders/Particle.vert
        shaders/Particle.geom
        shaders/Particle.frag
    )

# Headerize
add_subdirectory(externals/Headerize)
macro(ProcessWithHeaderize headervar sourcevar inputfile outdir)
    get_filename_component(basefile ${inputfile} NAME)
    set(outputfile ${outdir}/${basefile})
    set_source_files_properties(${outputfile} PROPERTIES HEADER_ONLY TRUE)
    add_custom_command(
        OUTPUT ${outputfile}.hpp ${outputfile}.cpp
        COMMAND Headerize ${inputfile} ${outputfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${inputfile}
        COMMENT "Generating headers for ${inputfile}"
    )
    list(APPEND ${headervar} ${outputfile}.hpp)
    list(APPEND ${sourcevar} ${outputfile}.cpp)
endmacro()

foreach(item ${EMBEDDED_INPUT})
    ProcessWithHeaderize(EMBEDDED_HEADER EMBEDDED_SOURCE ${item} ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

include_directories(src)

# Doxygen documentation
option(ModershipBuildDocs "Build the doxygen documentation" ON)
find_package(Doxygen)
if(DOXYGEN_FOUND AND ModershipBuildDocs)
    add_custom_target(EngineDoc ALL
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/EngineDoxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
    add_custom_target(GameDoc ALL
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/GameDoxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif()

## TARGETS ##

## Engine ##
set(ENGINE_SRCS
        src/Engine/MainWindow.cpp
        src/Engine/RenderTarget.cpp
        src/Engine/Resources.cpp
        src/Engine/Audio/SoundBuffer.cpp
        src/Engine/Audio/VorbisFile.cpp
        src/Engine/Audio/WaveFile.cpp
        src/Engine/Font/Font.cpp
        src/Engine/Geometry/Geometry2D.cpp
        src/Engine/Geometry/Square.cpp
        src/Engine/Geometry/Geometry3D.cpp
        src/Engine/Geometry/Terrain.cpp
        src/Engine/Geometry/Cube.cpp
        src/Engine/Geometry/Plane.cpp
        src/Engine/Geometry/OBJModel.cpp
        src/Engine/Geometry/Map.cpp
        src/Engine/Lighting/DeferredLighting.cpp
        src/Engine/Shader/Shader.cpp
        src/Engine/Shader/ShaderProgram.cpp
        src/Engine/Util/FileSystem.cpp
        src/Engine/Util/Log.cpp
        src/Engine/Util/Settings.cpp
        src/Engine/Util/Input.cpp
        src/Engine/Util/Picking.cpp
        src/Engine/Component/SuperComponent.cpp
        src/Engine/Component/Lens.cpp
        src/Engine/Component/Transform.cpp
        src/Engine/Component/Mesh.cpp
        src/Engine/Component/Material.cpp
        src/Engine/Component/RelativeTransform.cpp
        src/Engine/Component/Physics.cpp
        src/Engine/Component/Collider2DRectangle.cpp
        src/Engine/Component/Collider2DCircle.cpp
        src/Engine/Component/ParticleEmitter.cpp
        src/Engine/Component/DirectionalLight.cpp
        src/Engine/Component/PointLight.cpp
        src/Engine/Component/SpotLight.cpp
        src/Engine/Component/Animation.cpp
        src/Engine/Component/SoundSource.cpp
        src/Engine/Component/Listener.cpp
        src/Engine/Entity/Entity.cpp
        src/Engine/Scene/Scene.cpp
        src/Engine/System/RenderSystem.cpp
        src/Engine/System/PhysicsSystem.cpp
        src/Engine/System/CollisionSystem.cpp
        src/Engine/System/ParticleSystem.cpp
        src/Engine/System/ParticleRenderSystem.cpp
        src/Engine/System/AnimationSystem.cpp
        src/Engine/System/SoundSystem.cpp
        src/Engine/Texture/Texture.cpp
        src/Engine/Texture/Texture2D.cpp
        src/Engine/PostProcessing/PostProcessing.cpp
        src/Engine/PostProcessing/ColorFilter.cpp
        src/Engine/PostProcessing/FXAAFilter.cpp
        src/Engine/PostProcessing/FogFilter.cpp
        src/Engine/PostProcessing/GammaCorrectionFilter.cpp
        src/Engine/PostProcessing/GlowFilter.cpp
        src/Engine/PostProcessing/GlowBlurFilter.cpp
        src/Engine/GameObject/SuperGameObject.cpp
    )

set(ENGINE_HEADERS
        src/Engine/MainWindow.hpp
        src/Engine/RenderTarget.hpp
        src/Engine/Resources.hpp
        src/Engine/Audio/SoundBuffer.hpp
        src/Engine/Audio/SoundFile.hpp
        src/Engine/Audio/VorbisFile.hpp
        src/Engine/Audio/WaveFile.hpp
        src/Engine/Font/Font.hpp
        src/Engine/Geometry/Geometry2D.hpp
        src/Engine/Geometry/Square.hpp
        src/Engine/Geometry/Geometry3D.hpp
        src/Engine/Geometry/Terrain.hpp
        src/Engine/Geometry/Cube.hpp
        src/Engine/Geometry/Plane.hpp
        src/Engine/Geometry/OBJModel.hpp
        src/Engine/Geometry/Map.hpp
        src/Engine/Lighting/DeferredLighting.hpp
        src/Engine/Shader/Shader.hpp
        src/Engine/Shader/ShaderProgram.hpp
        src/Engine/Util/FileSystem.hpp
        src/Engine/Util/Log.hpp
        src/Engine/Util/Settings.hpp
        src/Engine/Util/Input.hpp
        src/Engine/Util/Picking.hpp
        src/Engine/Component/SuperComponent.hpp
        src/Engine/Component/Lens.hpp
        src/Engine/Component/Transform.hpp
        src/Engine/Component/Mesh.hpp
        src/Engine/Component/Material.hpp
        src/Engine/Component/RelativeTransform.hpp
        src/Engine/Component/Physics.hpp
        src/Engine/Component/Collider2DRectangle.hpp
        src/Engine/Component/Collider2DCircle.hpp
        src/Engine/Component/ParticleEmitter.hpp
        src/Engine/Component/DirectionalLight.hpp
        src/Engine/Component/PointLight.hpp
        src/Engine/Component/SpotLight.hpp
        src/Engine/Component/Animation.hpp
        src/Engine/Component/SoundSource.hpp
        src/Engine/Component/Listener.hpp
        src/Engine/Entity/Entity.hpp
        src/Engine/Scene/Scene.hpp
        src/Engine/System/RenderSystem.hpp
        src/Engine/System/PhysicsSystem.hpp
        src/Engine/System/CollisionSystem.hpp
        src/Engine/System/ParticleSystem.hpp
        src/Engine/System/ParticleRenderSystem.hpp
        src/Engine/System/AnimationSystem.hpp
        src/Engine/System/SoundSystem.hpp
        src/Engine/Texture/Texture.hpp
        src/Engine/Texture/Texture2D.hpp
        src/Engine/PostProcessing/PostProcessing.hpp
        src/Engine/PostProcessing/Filter.hpp
        src/Engine/PostProcessing/ColorFilter.hpp
        src/Engine/PostProcessing/FXAAFilter.hpp
        src/Engine/PostProcessing/FogFilter.hpp
        src/Engine/PostProcessing/GammaCorrectionFilter.hpp
        src/Engine/PostProcessing/GlowFilter.hpp
        src/Engine/PostProcessing/GlowBlurFilter.hpp
        src/Engine/GameObject/SuperGameObject.hpp
    )

create_directory_groups(${ENGINE_SRCS} ${ENGINE_HEADERS})

add_library(Engine STATIC ${ENGINE_SRCS} ${ENGINE_HEADERS} ${EMBEDDED_SOURCE})
set_property(TARGET Engine PROPERTY CXX_STANDARD 11)
set_property(TARGET Engine PROPERTY CXX_STANDARD_REQUIRED ON)

## Game ##
set(GAME_SRCS
        src/Game/main.cpp
        src/Game/Game.cpp
        src/Game/Util/GameSettings.cpp
        src/Game/Util/GameEntityFactory.cpp
        src/Game/Util/ControlSchemes.cpp
        src/Game/Util/Hub.cpp
        src/Game/Util/CaveGenerator.cpp
        src/Game/System/ControllerSystem.cpp
        src/Game/System/HealthSystem.cpp
        src/Game/System/DamageSystem.cpp
        src/Game/System/LifeTimeSystem.cpp
        src/Game/System/ExplodeSystem.cpp
        src/Game/System/ReflectSystem.cpp
        src/Game/System/AISystem.cpp
        src/Game/System/CheckpointSystem.cpp
        src/Game/System/EnemySpawnerSystem.cpp
        src/Game/System/UpdateSystem.cpp
        src/Game/System/GridCollideSystem.cpp
        src/Game/System/SpawnerSystem.cpp
        src/Game/Component/Controller.cpp
        src/Game/Component/Spawner.cpp
        src/Game/Component/Health.cpp
        src/Game/Component/Damage.cpp
        src/Game/Component/LifeTime.cpp
        src/Game/Component/Explode.cpp
        src/Game/Component/Reflect.cpp
        src/Game/Component/Update.cpp
        src/Game/Component/GridCollide.cpp
        src/Game/Component/AI/WalkerAI.cpp
        src/Game/Component/AI/LookerAI.cpp
        src/Game/GameScenes/MainScene.cpp
        src/Game/GameScenes/SplashScene.cpp
        src/Game/GameScenes/MenuScene.cpp
        src/Game/GameScenes/PontusScene.cpp
        src/Game/GameScenes/AlbinScene.cpp
        src/Game/GameScenes/DanielScene.cpp
        src/Game/GameScenes/IvarScene.cpp
        src/Game/GameScenes/JonathanScene.cpp
        src/Game/GameScenes/EmptyScene.cpp
        src/Game/GameScenes/WinScene.cpp
        src/Game/GameObject/Bullet.cpp
        src/Game/GameObject/Player/SuperPlayer.cpp
        src/Game/GameObject/Player/Player1.cpp
        src/Game/GameObject/Player/Player2.cpp
        src/Game/GameObject/Camera.cpp
        src/Game/GameObject/Cave.cpp
        src/Game/GameObject/Dust.cpp
        src/Game/GameObject/Explosion.cpp
        src/Game/GameObject/Shield.cpp
        src/Game/GameObject/EnemySpawner.cpp
        src/Game/GameObject/Enemy/SuperEnemy.cpp
        src/Game/GameObject/Enemy/Pylon.cpp
        src/Game/GameObject/Enemy/Rocket.cpp
        src/Game/GameObject/Boss/SuperBoss.cpp
        src/Game/GameObject/Boss/SpinBoss.cpp
        src/Game/GameObject/Boss/ShieldBoss.cpp
        src/Game/GameObject/Boss/DivideBoss.cpp
        src/Game/GameObject/Boss/RingBoss.cpp
        src/Game/GameObject/Altar.cpp
        src/Game/GameObject/Pillar.cpp
        src/Game/GameObject/PillarBall.cpp
        src/Game/GameObject/Portal.cpp
    )

set(GAME_HEADERS
        src/Game/Game.hpp
        src/Game/Util/GameSettings.hpp
        src/Game/Util/GameSettings.hpp
        src/Game/Util/GameEntityFactory.hpp
        src/Game/Util/ControlSchemes.hpp
        src/Game/Util/Hub.hpp
        src/Game/Util/CaveGenerator.hpp
        src/Game/System/ControllerSystem.hpp
        src/Game/System/HealthSystem.hpp
        src/Game/System/DamageSystem.hpp
        src/Game/System/LifeTimeSystem.hpp
        src/Game/System/ExplodeSystem.hpp
        src/Game/System/ReflectSystem.hpp
        src/Game/System/AISystem.hpp
        src/Game/System/CheckpointSystem.hpp
        src/Game/System/EnemySpawnerSystem.hpp
        src/Game/System/UpdateSystem.hpp
        src/Game/System/GridCollideSystem.hpp
        src/Game/System/SpawnerSystem.hpp
        src/Game/Component/Controller.hpp
        src/Game/Component/Spawner.hpp
        src/Game/Component/Health.hpp
        src/Game/Component/Damage.hpp
        src/Game/Component/LifeTime.hpp
        src/Game/Component/Explode.hpp
        src/Game/Component/Reflect.hpp
        src/Game/Component/Update.hpp
        src/Game/Component/GridCollide.hpp
        src/Game/Component/AI/WalkerAI.hpp
        src/Game/Component/AI/LookerAI.hpp
        src/Game/GameScenes/MainScene.hpp
        src/Game/GameScenes/SplashScene.hpp
        src/Game/GameScenes/MenuScene.hpp
        src/Game/GameScenes/PontusScene.hpp
        src/Game/GameScenes/AlbinScene.hpp
        src/Game/GameScenes/DanielScene.hpp
        src/Game/GameScenes/IvarScene.hpp
        src/Game/GameScenes/JonathanScene.hpp
        src/Game/GameScenes/EmptyScene.hpp
        src/Game/GameScenes/WinScene.hpp
        src/Game/GameObject/Bullet.hpp
        src/Game/GameObject/Player/SuperPlayer.hpp
        src/Game/GameObject/Player/Player1.hpp
        src/Game/GameObject/Player/Player2.hpp
        src/Game/GameObject/Camera.hpp
        src/Game/GameObject/Cave.hpp
        src/Game/GameObject/Dust.hpp
        src/Game/GameObject/Explosion.hpp
        src/Game/GameObject/Shield.hpp
        src/Game/GameObject/EnemySpawner.hpp
        src/Game/GameObject/Enemy/SuperEnemy.hpp
        src/Game/GameObject/Enemy/Pylon.hpp
        src/Game/GameObject/Enemy/Rocket.hpp
        src/Game/GameObject/Boss/SuperBoss.hpp
        src/Game/GameObject/Boss/SpinBoss.hpp
        src/Game/GameObject/Boss/ShieldBoss.hpp
        src/Game/GameObject/Boss/DivideBoss.hpp
        src/Game/GameObject/Boss/RingBoss.hpp
        src/Game/GameObject/Altar.hpp
        src/Game/GameObject/Pillar.hpp
        src/Game/GameObject/PillarBall.hpp
        src/Game/GameObject/Portal.hpp
    )

create_directory_groups(${GAME_SRCS} ${GAME_HEADERS})

add_executable(Modership ${GAME_SRCS} ${GAME_HEADERS})
target_link_libraries(Modership Engine)
target_include_directories(Modership PUBLIC src/Engine)
target_link_libraries(Modership glew glfw ${GLFW_LIBRARIES} ${OPENAL_LIB})
set_property(TARGET Modership PROPERTY CXX_STANDARD 11)
set_property(TARGET Modership PROPERTY CXX_STANDARD_REQUIRED ON)
