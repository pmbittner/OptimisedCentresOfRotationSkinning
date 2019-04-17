cmake_minimum_required(VERSION 3.12)
project(CoRCalculator)

set(CMAKE_CXX_STANDARD 11)

# Add the pthread flag, so that we will be able to use std::async
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -pthread)

# Include the glm library.
# If you are on Windows, please change this path to your version of glm.
if (WIN32)
    SET(LIBRARIES_BASE_PATH "D:/ThirdPartyLibraries")
    include_directories("${LIBRARIES_BASE_PATH}/GLM/glm-0.9.7.6")
endif(WIN32)

include_directories(include)

set(COR_HEADERS include/cor/CoRCalculator.h include/cor/WeightsPerBone.h include/cor/CoRTriangle.h include/cor/CoRMesh.h include/cor/Clock.h)
set(COR_SOURCES src/CoRCalculator.cpp src/CoRTriangle.cpp src/Clock.cpp src/WeightsPerBone.cpp)

add_library(CoRCalculator ${COR_HEADERS} ${COR_SOURCES})

add_executable(CoRCalculationExample example.cpp)
target_link_libraries(CoRCalculationExample CoRCalculator)