cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME c8t6-test-FOC)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM CXX)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

## 添加我的源文件
file(GLOB_RECURSE USER_SOURCES 
    # ${PROJECT_SOURCE_DIR}/lib/Arduino-FOC/*.c
    # ${PROJECT_SOURCE_DIR}/lib/Arduino-FOC/*.cpp
    ${PROJECT_SOURCE_DIR}/lib/simpleFOC-next/*.c
    ${PROJECT_SOURCE_DIR}/lib/simpleFOC-next/*.cpp
)
message(STATUS "Source Files: ${USER_SOURCES}")
# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${USER_SOURCES}
)

## 添加我的头文件
# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    # ${PROJECT_SOURCE_DIR}/lib/Arduino-FOC/
    # ${PROJECT_SOURCE_DIR}/lib/Arduino-FOC/common/base_classes/
    # ${PROJECT_SOURCE_DIR}/lib/Arduino-FOC/common/
    # ${PROJECT_SOURCE_DIR}/lib/Arduino-FOC/drivers/
    ${PROJECT_SOURCE_DIR}/lib/simpleFOC-next/
    ${PROJECT_SOURCE_DIR}/lib/simpleFOC-next/common/
    ${PROJECT_SOURCE_DIR}/lib/simpleFOC-next/common/base_classes/
    ${PROJECT_SOURCE_DIR}/lib/simpleFOC-next/drivers/
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols

)

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    # _STM32_DEF_
    )

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
