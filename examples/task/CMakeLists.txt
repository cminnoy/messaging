cmake_minimum_required(VERSION 3.14.0)
project(task LANGUAGES CXX VERSION 1.0.0)

cmake_policy(SET CMP0063 NEW)
set(CMAKE_VISIBILITY_INLINES_HIDDEN FALSE)
set(CMAKE_C_VISIBILITY_PRESET default)
set(CMAKE_CXX_VISIBILITY_PRESET default)

add_executable(${PROJECT_NAME} main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET default)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ ${CMAKE_CURRENT_SOURCE_DIR}/../include)
link_directories(${CMAKE_BINARY_DIR})
target_link_libraries(${PROJECT_NAME} messaging)

add_definitions(-DALLOW_CHANGE_THREAD_NAME)

if(WITH_TRACY)
target_link_directories(${PROJECT_NAME} PRIVATE ${tracy_BINARY_DIR})
target_link_libraries(${PROJECT_NAME} TracyClient)
endif()
