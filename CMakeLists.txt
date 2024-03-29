###############################################################################
cmake_minimum_required(VERSION 3.0)
project(VVR_OGL_LABORATORY)

###############################################################################

find_package(OpenGL REQUIRED)

# c++11, -g option is used to export debug symbols for gdb
if(${CMAKE_CXX_COMPILER_ID} MATCHES GNU OR
    ${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
  # Using C++11 on OSX requires using libc++ instead of libstd++.
  # libc++ is an implementation of the C++ standard library for OSX.
  if(APPLE)
    if(XCODE)
      set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
      set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
    endif()
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
  endif()
endif()

# define library postfix so that different builds will produce
# distinguished libraries
set(CMAKE_RELEASE_POSTFIX "_r" CACHE string "Release postfix")
set(CMAKE_DEBUG_POSTFIX "_d" CACHE string "Debug postfix")
set(CMAKE_RELWITHDEBINFO_POSTFIX "_rd" CACHE string
  "Release with debug info postfix")
set(CMAKE_MINSIZEREL_POSTFIX "_mr" CACHE string
  "Minimum size release postfix")
mark_as_advanced(CMAKE_RELEASE_POSTFIX)
mark_as_advanced(CMAKE_DEBUG_POSTFIX)
mark_as_advanced(CMAKE_RELWITHDEBINFO_POSTFIX)
mark_as_advanced(CMAKE_MINSIZEREL_POSTFIX)

# for rdm (emacs)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# for grouping into folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
  message(FATAL_ERROR "Please select another Build Directory ! (and give it a
  clever name, like bin_Visual2012_64bits/)")
endif()
if(CMAKE_SOURCE_DIR MATCHES " ")
  message( "Your Source Directory contains spaces. If you experience problems
    when compiling, this can be the cause." )
endif()
if(CMAKE_BINARY_DIR MATCHES " ")
  message("Your Build Directory contains spaces. If you experience problems
  when compiling, this can be the cause.")
endif()

# Compile external dependencies
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP


###############################################################################

include_directories(
  external/glfw-3.2.1/include/GLFW/
  external/glm-0.9.7.1/
  external/glew-1.13.0/include/
  external/Simple-OpenGL-Image-Library/include/
  external/tinyxml2/include/
  external/tinyobjloader/include/
  src/components/
  .
  )

set(ALL_LIBS
  ${OPENGL_LIBRARY}
  GLEW_1130
  glfw
  SOIL
  TINYXML2
  )

add_definitions(
  -DTW_STATIC
  -DTW_NO_LIB_PRAGMA
  -DTW_NO_DIRECT3D
  -DGLEW_STATIC
  -D_CRT_SECURE_NO_WARNINGS
  )

###############################################################################
# source files
add_executable(art_gallery
  src/main.cpp
  src/constants.h

  common/util.cpp
  common/util.h
  common/shader.cpp
  common/shader.h
  common/camera.cpp
  common/camera.h
  common/model.cpp
  common/model.h
  common/texture.cpp
  common/texture.h

  src/components/object/object.h
  src/components/object/object.cpp
  
  src/components/rooms/room.h
  src/components/rooms/room.cpp

  src/components/rooms/main_room.h
  src/components/rooms/main_room.cpp
  src/components/rooms/secondary_room.h
  src/components/rooms/secondary_room.cpp

  src/components/floor/floor.h
  src/components/floor/floor.cpp
  src/components/wall/wall.h
  src/components/wall/wall.cpp

  src/components/paintings/paintings.h
  src/components/paintings/paintings.cpp

  src/components/player/player.h
  src/components/player/player.cpp

  src/components/light/light.h
  src/components/light/light.cpp

  src/post_processing/floyd_steinberg.h
  src/post_processing/floyd_steinberg.cpp
  src/post_processing/painterly.h
  src/post_processing/painterly.cpp

  src/FBOs/FBO.h
  src/FBOs/FBO.cpp

  src/FBOs/sceneFBO/sceneFBO.h
  src/FBOs/sceneFBO/sceneFBO.cpp
  src/FBOs/depthFBO/depthFBO.h
  src/FBOs/depthFBO/depthFBO.cpp
  src/FBOs/paintingsFBO/paintingsFBO.h
  src/FBOs/paintingsFBO/paintingsFBO.cpp
  src/FBOs/renderFBO/renderFBO.h
  src/FBOs/renderFBO/renderFBO.cpp

  src/rendering/main_loop.h
  src/rendering/main_loop.cpp
  src/rendering/final_render.h
  src/rendering/final_render.cpp
  src/rendering/bump_render.h
  src/rendering/bump_render.cpp
  src/rendering/disp_render.h
  src/rendering/disp_render.cpp
  )
  
target_link_libraries(art_gallery
  ${ALL_LIBS}
  )
# Add compialer optimizations
target_compile_options(art_gallery PRIVATE -O1)

# Xcode and Visual working directories
set_target_properties(art_gallery
  PROPERTIES
  XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/art_gallery/"
  PROJECT_LABEL "Art Gallery Project"
  FOLDER "Exercise"
  )
create_target_launcher(art_gallery WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/art_gallery/")
create_default_target_launcher(art_gallery WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/art_gallery/")

###############################################################################

SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )
SOURCE_GROUP(obj REGULAR_EXPRESSION ".*/.*obj$" )

###############################################################################
# copy
if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

  add_custom_command(
    TARGET art_gallery POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/art_gallery${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/src/"
    )
    
elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )


message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C compiler: ${CMAKE_C_COMPILER}")