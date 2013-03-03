cmake_minimum_required(VERSION 2.8)

include(FRCDeploy)

project(GUILLOTINE)

file(GLOB_RECURSE SOURCE_CODE *.cpp)

find_package(WPILib)

include_directories(${WPILib_INCLUDE_DIRS})

add_executable(612-code ${SOURCE_CODE})

target_link_libraries(612-code ${WPILib_LIBRARY})

add_make_deploy(612-code 10.6.12.2)
