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

# Copy data and posters folder to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/posters
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/posters
)
