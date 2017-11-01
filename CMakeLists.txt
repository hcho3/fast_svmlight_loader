cmake_minimum_required (VERSION 3.1)
project(fast_svmlight_loader)
include(cmake/Utils.cmake)
find_package(OpenMP)

# enable custom logging facility in dmlc-core
add_definitions(-DDMLC_LOG_CUSTOMIZE)

# check MSVC version
if(MSVC)
  if(MSVC_VERSION LESS 1900)
    message(FATAL_ERROR "Need Visual Studio 2015 or newer to compile fast_svmlight_loader")
  endif()
endif()

set_default_configuration_release()
msvc_use_static_runtime()

# OpenMP check
if(OPENMP_FOUND)
  message("Got OpenMP")
  add_definitions(-DFAST_SVMLIGHT_LOADER_OPENMP_SUPPORT)
else()
  message("Don't have OpenMP")
  include(dmlc-core/cmake/Utils.cmake)
  dmlccore_option(USE_OPENMP "Build with OpenMP" OFF)
endif()

# Compiler flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if(MSVC)
  # Multithreaded compilation
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
else()
  # Performance
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -funroll-loops")
endif()

include_directories (
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/dmlc-core/include
)

file(GLOB_RECURSE SOURCES
    src/*.cc
    src/*.h
    include/*.h
)

# dmlc-core
add_subdirectory(dmlc-core)
# separate sets of dependencies for main package and runtime
set(LINK_LIBRARIES dmlc)
add_library(obj_fast_svmlight_loader OBJECT ${SOURCES})

# Shared library
add_library(fast_svmlight_loader SHARED $<TARGET_OBJECTS:obj_fast_svmlight_loader>)
target_link_libraries(fast_svmlight_loader ${LINK_LIBRARIES})
set_output_directory(fast_svmlight_loader ${PROJECT_SOURCE_DIR}/lib)
if(MINGW)
  # remove the 'lib' prefix to conform to windows convention for shared library names
  set_target_properties(fast_svmlight_loader PROPERTIES PREFIX "")
endif()

# Group sources
auto_source_group("${SOURCES}")