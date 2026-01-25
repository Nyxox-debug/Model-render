cmake_minimum_required(VERSION 3.10)
project(3dWorld LANGUAGES C CXX)

# NOTE: I set cxx stand and compilation_db for cland in CMakePresets

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(assimp REQUIRED)

# GLAD 
add_library(glad
  external/glad/src/glad.c
)

target_include_directories(glad
  PUBLIC
    external/glad/include
)

if(UNIX AND NOT APPLE)
  target_link_libraries(glad PRIVATE dl)
endif()

# Application 
add_executable(app
  src/main.cpp
  src/engine.cpp
  src/model/mesh.cpp
  src/model/model.cpp
  src/shader/shader.cpp
)

target_link_libraries(app
  PRIVATE
    glad
    glfw
    OpenGL::GL
    assimp::assimp
)

# GLM
# FIX: Not sure yet if glad, khr and glm should be in include dir 
include_directories(${CMAKE_SOURCE_DIR}/include)

# NOTE: If I want to make assimp available to my includes, but modern cpp handles this automatically
# target_include_directories(app PRIVATE ${ASSIMP_INCLUDE_DIRS})
