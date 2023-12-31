cmake_minimum_required(VERSION 3.27)

project(VoxelEngine VERSION 1.0
                    DESCRIPTION "A voxel game engine"
                    LANGUAGES C CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    include(CTest)

    find_package(Doxygen)
    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()

endif()

include(FetchContent)

add_subdirectory(src)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR VOXEL_ENGINE_BUILD_TESTING) AND BUILD_TESTING)
    add_subdirectory(tests)
endif()

