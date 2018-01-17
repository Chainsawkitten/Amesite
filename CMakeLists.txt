cmake_minimum_required(VERSION 2.8.12)

project(Amesite)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "-g -gdwarf-2 -O3 -static-libstdc++ -Wall")
endif()

# Externals
add_subdirectory(externals)

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

# Copy resources
add_custom_target(ResourceTarget ALL)
add_custom_command(
    TARGET ResourceTarget
    COMMAND ${CMAKE_COMMAND} -E copy_directory assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Resources
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Copying assets."
)

foreach(item ${RESOURCES})
    CopyAsset(${item} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Resources)
endforeach()

# Doxygen documentation
option(AMESITE_BUILD_DOCS "Build the doxygen documentation" OFF)
find_package(Doxygen)
if(DOXYGEN_FOUND AND AMESITE_BUILD_DOCS)
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
add_subdirectory(src)

## Game ##
set(GAME_SRCS
        src/Game/main.cpp
        src/Game/Game.cpp
        src/Game/Util/GameSettings.cpp
        src/Game/Util/PerlinNoise.cpp
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
        src/Game/GameScenes/MainScene.cpp
        src/Game/GameScenes/SplashScene.cpp
        src/Game/GameScenes/EmptyScene.cpp
        src/Game/GameScenes/WinScene.cpp
        src/Game/GameScenes/LoadingScene.cpp
        src/Game/GameObject/Bullet.cpp
        src/Game/GameObject/Player/SuperPlayer.cpp
        src/Game/GameObject/Player/Player1.cpp
        src/Game/GameObject/Player/Player2.cpp
        src/Game/GameObject/Camera.cpp
        src/Game/GameObject/Cave.cpp
        src/Game/GameObject/Dust.cpp
        src/Game/GameObject/Explosion.cpp
        src/Game/GameObject/DyingLight.cpp
        src/Game/GameObject/Shield.cpp
        src/Game/GameObject/EnemySpawner.cpp
        src/Game/GameObject/Enemy/SuperEnemy.cpp
        src/Game/GameObject/Enemy/Pylon.cpp
        src/Game/GameObject/Enemy/Rocket.cpp
        src/Game/GameObject/Enemy/Nest.cpp
        src/Game/GameObject/Boss/SuperBoss.cpp
        src/Game/GameObject/Boss/SpinBoss.cpp
        src/Game/GameObject/Boss/ShieldBoss.cpp
        src/Game/GameObject/Boss/DivideBoss.cpp
        src/Game/GameObject/Boss/RingBoss.cpp
        src/Game/GameObject/Altar.cpp
        src/Game/GameObject/Pillar.cpp
        src/Game/GameObject/PillarBall.cpp
        src/Game/GameObject/Portal.cpp
        src/Game/GameObject/ReviveCircle.cpp
        src/Game/Menu/Menu.cpp
        src/Game/Menu/SubMenu.cpp
        src/Game/Menu/MainMenu.cpp
        src/Game/Menu/OptionsMenu.cpp
        src/Game/Menu/PauseMenu.cpp
        src/Game/Menu/SuperMenuOption.cpp
        src/Game/Menu/MenuOption.cpp
        src/Game/Menu/MenuSwitch.cpp
        src/Game/Water/Water.cpp
    )

set(GAME_HEADERS
        src/Game/Game.hpp
        src/Game/Util/GameSettings.hpp
        src/Game/Util/PerlinNoise.hpp
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
        src/Game/GameScenes/MainScene.hpp
        src/Game/GameScenes/SplashScene.hpp
        src/Game/GameScenes/EmptyScene.hpp
        src/Game/GameScenes/WinScene.hpp
        src/Game/GameScenes/LoadingScene.hpp
        src/Game/GameObject/Bullet.hpp
        src/Game/GameObject/Player/SuperPlayer.hpp
        src/Game/GameObject/Player/Player1.hpp
        src/Game/GameObject/Player/Player2.hpp
        src/Game/GameObject/Camera.hpp
        src/Game/GameObject/Cave.hpp
        src/Game/GameObject/Dust.hpp
        src/Game/GameObject/Explosion.hpp
        src/Game/GameObject/DyingLight.hpp
        src/Game/GameObject/Shield.hpp
        src/Game/GameObject/EnemySpawner.hpp
        src/Game/GameObject/Enemy/SuperEnemy.hpp
        src/Game/GameObject/Enemy/Pylon.hpp
        src/Game/GameObject/Enemy/Rocket.hpp
        src/Game/GameObject/Enemy/Nest.hpp
        src/Game/GameObject/Boss/SuperBoss.hpp
        src/Game/GameObject/Boss/SpinBoss.hpp
        src/Game/GameObject/Boss/ShieldBoss.hpp
        src/Game/GameObject/Boss/DivideBoss.hpp
        src/Game/GameObject/Boss/RingBoss.hpp
        src/Game/GameObject/Altar.hpp
        src/Game/GameObject/Pillar.hpp
        src/Game/GameObject/PillarBall.hpp
        src/Game/GameObject/Portal.hpp
        src/Game/GameObject/ReviveCircle.hpp
        src/Game/Menu/Menu.hpp
        src/Game/Menu/SubMenu.hpp
        src/Game/Menu/MainMenu.hpp
        src/Game/Menu/OptionsMenu.hpp
        src/Game/Menu/PauseMenu.hpp
        src/Game/Menu/SuperMenuOption.hpp
        src/Game/Menu/MenuOption.hpp
        src/Game/Menu/MenuSwitch.hpp
        src/Game/Water/Water.hpp
    )

create_directory_groups(${GAME_SRCS} ${GAME_HEADERS})

add_executable(Amesite ${GAME_SRCS} ${GAME_HEADERS})
target_link_libraries(Amesite Engine)
target_include_directories(Amesite PUBLIC src/Engine)
set_property(TARGET Amesite PROPERTY CXX_STANDARD 11)
set_property(TARGET Amesite PROPERTY CXX_STANDARD_REQUIRED ON)
