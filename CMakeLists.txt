cmake_minimum_required(VERSION 3.16)
project(PBL2-Cinema-Booking-System LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

include_directories(include ${CMAKE_SOURCE_DIR})

file(GLOB SOURCES 
    "src/models/*.cpp"
    "src/utils/*.cpp"
    "src/managers/*.cpp"
    "ui/login/*.cpp"
    "ui/customer/*.cpp"
    "ui/cashier/*.cpp"
    "ui/admin/*.cpp"
    "ui/common/*.cpp"
    "src/main.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

# Always synchronize data and posters folder to build directory on every build
add_custom_target(copy_data ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        ${CMAKE_CURRENT_BINARY_DIR}/data
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/posters
        ${CMAKE_CURRENT_BINARY_DIR}/posters
)
