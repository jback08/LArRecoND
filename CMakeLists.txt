# cmake file for building LArRecoND
#-------------------------------------------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.2 FATAL_ERROR)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "LArRecoND requires an out-of-source build.")
endif()

# project name
project(LArRecoND)

# project version
set(${PROJECT_NAME}_VERSION_MAJOR 01)
set(${PROJECT_NAME}_VERSION_MINOR 02)
set(${PROJECT_NAME}_VERSION_PATCH 02)
set(${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}")

#-------------------------------------------------------------------------------------------------------------------------------------------
# Dependencies
include(PandoraCMakeSettings)

# Prefer local include directory to any paths to installed header files
include_directories(include)

find_package(PandoraSDK 03.04.01 REQUIRED)

set(LAR_CONTENT_LIBRARY_NAME "LArContent")
set(LAR_CONTENT_LIBRARY_VERSION "04.14.00")
find_package(${LAR_CONTENT_LIBRARY_NAME} ${LAR_CONTENT_LIBRARY_VERSION} REQUIRED)

if(PANDORA_LIBTORCH)
    set(LAR_DL_CONTENT_LIBRARY_NAME "LArDLContent")
    find_package(${LAR_DL_CONTENT_LIBRARY_NAME} ${LAR_CONTENT_LIBRARY_VERSION} REQUIRED)
    add_definitions(-DLIBTORCH_DL=1)
endif()

foreach(package PandoraSDK ${LAR_CONTENT_LIBRARY_NAME} ${LAR_DL_CONTENT_LIBRARY_NAME})
    if(${package}_FOUND)
        include_directories(${${package}_INCLUDE_DIRS})
        link_libraries(${${package}_LIBRARIES})
        add_definitions (${${package}_DEFINITIONS})
    endif()
endforeach()

if(PANDORA_MONITORING)
    find_package(PandoraMonitoring 03.06.00 REQUIRED)
    include_directories(${PandoraMonitoring_INCLUDE_DIRS})
    link_libraries(${PandoraMonitoring_LIBRARIES})
    add_definitions(${PandoraMonitoring_DEFINITIONS})
    add_definitions("-DMONITORING")

    list(APPEND CMAKE_MODULE_PATH "$ENV{ROOTSYS}/etc/cmake/")
    find_package(ROOT 6.18.04 REQUIRED COMPONENTS Eve Geom RGL EG)
endif()

if(USE_EDEPSIM)
  find_package(EDepSim)
  add_definitions("-DUSE_EDEPSIM")
endif()

#-------------------------------------------------------------------------------------------------------------------------------------------
# Low level settings - compiler etc
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -Wno-long-long -Wno-sign-compare -Wshadow -fno-strict-aliasing -std=c++17 ${CMAKE_CXX_FLAGS}")

include(CheckCXXCompilerFlag)
unset(COMPILER_SUPPORTS_CXX_FLAGS CACHE)
CHECK_CXX_COMPILER_FLAG(${CMAKE_CXX_FLAGS} COMPILER_SUPPORTS_CXX_FLAGS)

if(NOT COMPILER_SUPPORTS_CXX_FLAGS)
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} does not support cxx flags ${CMAKE_CXX_FLAGS}")
endif()

#-------------------------------------------------------------------------------------------------------------------------------------------
# Build products

# - Collect sources - not ideal because you have to keep running CMake to pick up changes
file(GLOB_RECURSE LAR_RECO_ND_SRCS RELATIVE ${PROJECT_SOURCE_DIR} "src/*.cc")

# - Add library and properties
add_library(${PROJECT_NAME} SHARED ${LAR_RECO_ND_SRCS})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${${PROJECT_NAME}_VERSION} SOVERSION ${${PROJECT_NAME}_SOVERSION})

# - Executable
add_executable(PandoraInterface ${PROJECT_SOURCE_DIR}/test/PandoraInterface.cxx)
if(PANDORA_MONITORING)
    include_directories(${ROOT_INCLUDE_DIRS})
    if(USE_EDEPSIM)
        target_link_libraries(PandoraInterface ${ROOT_LIBRARIES} EDepSim::edepsim_io)
    else()
        target_link_libraries(PandoraInterface ${ROOT_LIBRARIES})
    endif()
endif()
target_link_libraries(PandoraInterface ${PROJECT_NAME})

# - Optional documents
option(LArRecoND_BUILD_DOCS "Build documentation for ${PROJECT_NAME}" OFF)
if(LArRecoND_BUILD_DOCS)
    add_subdirectory(doc)
endif()

#-------------------------------------------------------------------------------------------------------------------------------------------
# Install products

# - library
install(TARGETS ${PROJECT_NAME} DESTINATION lib COMPONENT Runtime)

# - headers
install(DIRECTORY include/ DESTINATION include COMPONENT Development FILES_MATCHING PATTERN "*.h")

# - executable
install(TARGETS PandoraInterface DESTINATION bin PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

#-------------------------------------------------------------------------------------------------------------------------------------------
# display some variables and write them to cache
PANDORA_DISPLAY_STD_VARIABLES()
