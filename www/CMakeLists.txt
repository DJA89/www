cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0015 NEW)

execute_process(COMMAND ln -s ../shaders)
execute_process(COMMAND ln -s ../images)
execute_process(COMMAND ln -s ../levels)
execute_process(COMMAND ln -s ../sounds)
#execute_process(COMMAND ln -s ../lib)

set(appName Bubble)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(SOIL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/inc)
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${SOIL_INCLUDE_DIRS})

link_directories(${CMAKE_SOURCE_DIR}/lib)
link_directories(${OPENGL_LIBRARY_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
link_directories(${SOIL_LIBRARY_DIRS})

add_executable(${appName} DeathTile.h DeathTile.cpp Checkpoint.h Checkpoint.cpp SoundSystem.h SoundSystem.cpp Entity.h Entity.cpp FixedPathEntity.h FixedPathEntity.cpp AxisAlignedBoundingBox.h AxisAlignedBoundingBox.cpp BoundingShape.h BoundingEllipse.h BoundingEllipse.cpp Intersection.h Intersection.cpp TileType.h TileType.cpp Utils.h Utils.cpp SavedState.h SavedState.cpp LevelMap.h LevelMap.cpp lib/tinyxml2.h lib/tinyxml2.cpp Player.h Player.cpp AnimKeyframes.h Sprite.h Sprite.cpp TileMap.h TileMap.cpp Texture.h Texture.cpp Scene.h Scene.cpp Shader.h Shader.cpp ShaderProgram.h ShaderProgram.cpp Game.h Game.cpp main.cpp)

target_link_libraries(${appName} fmod ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${SOIL_LIBRARIES} ${FREETYPE_LIBRARIES}) # ${GLM_LIBRARIES}
