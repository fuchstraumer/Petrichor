cmake_minimum_required(VERSION 3.6)
project(Petrichor)

find_package(Vulkan REQUIRED QUIET)
add_subdirectory(submodules/vulpesrender)

set(petrichor_sources
    "${CMAKE_CURRENT_SOURCE_DIR}/include/PetrichorAPI.hpp"
    #"${CMAKE_CURRENT_SOURCE_DIR}/include/PetrichorResourceTypes.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/PlatformWindow.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/RenderingContext.hpp"
    #"${CMAKE_CURRENT_SOURCE_DIR}/include/ResourceContext.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mwsrQueue.hpp"
    #"${CMAKE_CURRENT_SOURCE_DIR}/src/PetrichorResourceTypes.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/PlatformWindow.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/RenderingContext.cpp")
    #"${CMAKE_CURRENT_SOURCE_DIR}/src/ResourceContext.cpp"
    #"${CMAKE_CURRENT_SOURCE_DIR}/src/ResourceContextImpl.hpp"
    #"${CMAKE_CURRENT_SOURCE_DIR}/src/ResourceContextImpl.cpp"
    #"${CMAKE_CURRENT_SOURCE_DIR}/src/ResourceCreationCoro.hpp"
    #"${CMAKE_CURRENT_SOURCE_DIR}/src/ResourceCreationCoro.cpp")

option(PETRICHOR_VALIDATION_ENABLED_CONF "Enable validation layer for rendering context" ON)
option(PETRICHOR_DEBUG_INFO_ENABLED_CONF "Enable debug info for objects created by the rendering context" ON)
option(PETRICHOR_DEBUG_INFO_THREADING_ENABLED_CONF "Attach thread ID to debug info" ON)
# Currently WIP
option(PETRICHOR_DEBUG_INFO_TIMESTAMPS_ENABLED_CONF "Attach timestamp to debug info" OFF)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/RenderingContextConfig.hpp.in"
    "RenderingContextConfig.hpp")

# Will add shared library support properly, at a future date
add_library(petrichor STATIC ${petrichor_sources})
target_link_libraries(petrichor PUBLIC vpr_core glfw ${Vulkan_LIBRARY} vpr_sync)
target_include_directories(petrichor PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/submodules/vulpesrender/command/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/submodules/vulpesrender/common"
    "${CMAKE_CURRENT_SOURCE_DIR}/submodules/vulpesrender/core/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/submodules/vulpesrender/render/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/submodules/vulpesrender/resource/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/submodules/vulpesrender/sync/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/submodules/json/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/submodules/vma/src"
    "${Vulkan_INCLUDE_DIR}")

if(MSVC)
    target_compile_options(petrichor PRIVATE "/std:c++latest")
else()
    set_target_properties(petrichor PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED YES)
endif()

set(petrichor_base_test_sources
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/TestSceneFramework.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/TestSceneFramework.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/CommonCreationFunctions.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/CommonCreationFunctions.cpp")

# Should inherit most of these from linking to petrichor
set(petrichor_test_include_dirs
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/"
    "${CMAKE_CURRENT_SOURCE_DIR}/submodules/glm/"
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/")

function(add_petrichor_test NAME)
    add_executable(${NAME} ${petrichor_base_test_sources} ${ARGN})
    target_include_directories(${NAME} PRIVATE ${petrichor_test_include_dirs})
    target_link_libraries(${NAME} PRIVATE petrichor)
    if (MSVC)
        target_compile_options(${NAME} PRIVATE "/std:c++latest")
        target_compile_definitions(${NAME} PUBLIC
        "NOMINMAX" "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS")
        target_compile_options(${NAME} PRIVATE $<$<CONFIG:RELEASE>:/Oi>)
    else()
        set_target_properties(${NAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED YES)
    endif()
    set_target_properties(${NAME} PROPERTIES FOLDER "Petrichor Tests")
    ADD_CUSTOM_COMMAND(TARGET ${NAME} POST_BUILD COMMAND ${CMAKE_COMMAND}
        -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/cfg/" "${CMAKE_CURRENT_BINARY_DIR}/")
endfunction()

option(PETRICHOR_BUILD_TESTS "Build a series of test executables used to verify core functionality" OFF)
if(PETRICHOR_BUILD_TESTS)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests/RenderingContextTest")
endif()
