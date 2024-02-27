#cmake - format off
cmake_minimum_required(VERSION 3.16.5)
project(messaging LANGUAGES CXX VERSION 1.0.0)

# message("Your C++ compiler supports these C++ features:")
# foreach(i ${CMAKE_CXX_COMPILE_FEATURES})
#   message("${i}")
# endforeach()

include(FetchContent)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

cmake_policy(SET CMP0063 NEW)
set(CMAKE_VISIBILITY_INLINES_HIDDEN FALSE)
set(CMAKE_C_VISIBILITY_PRESET default)
set(CMAKE_CXX_VISIBILITY_PRESET default)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(SDKTARGETSYSROOT $ENV{SDKTARGETSYSROOT})
if (SDKTARGETSYSROOT)
    # need proot to run against sdk root
    set(PROOT proot  -R  ${SDKTARGETSYSROOT} ${PROOT_ARGS})
    link_libraries("-Wl,-rpath,$ENV{SDKTARGETSYSROOT}/usr/${CMAKE_INSTALL_LIBDIR}")
endif ()

if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 12.1)
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -faligned-new -ffold-simple-inlines")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wdelete-non-virtual-dtor -Winline -Wsuggest-final-types -Wnoexcept -Wsuggest-final-methods -Wsuggest-override -Wextra -faligned-new -grecord-gcc-switches -fconcepts-diagnostics-depth=10")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -faligned-new")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wdelete-non-virtual-dtor -Winline -Wsuggest-final-types -Wnoexcept -Wsuggest-final-methods -Wsuggest-override -Wextra -faligned-new -grecord-gcc-switches")
endif()

option(WITH_TRACY "Use Tracy profiler support for allocator." OFF)
option(WITH_TRACER_SUPPORT "Enable TRACER_FUNC support." OFF)
option(WITH_MESSAGE_SET_THREAD_NAME "Allow system threads to follow the process name." OFF)
option(WITH_MESSAGE_SET_THREAD_PRIORITY "Allow system threads to have their priority changed." OFF)
option(WITH_MESSAGE_SET_THREAD_AFFINITY "Allow system threads to be affiniated to specific cores." OFF)
option(WITH_TESTS "Build test files" ON)
option(WITH_EXAMPLES "Build example files" ON)
option(WITH_GCOV "Build test files with coverage mode enabled" OFF)
option(WITH_BOOST_COROUTINES "Build library with BOOST coroutines" ON)
set(CPP_STANDARD "C++20" CACHE STRING "C++ standard to use")

if("${CPP_STANDARD}" STREQUAL "C++11")
    set(CMAKE_CXX_STANDARD 11) # C++11 not supported yet, may be used for a minimal library; most code needs C++14 at least
elseif ("${CPP_STANDARD}" STREQUAL "C++14")
    set(CMAKE_CXX_STANDARD 14)
elseif ("${CPP_STANDARD}" STREQUAL "C++17")
    set(CMAKE_CXX_STANDARD 17)
elseif ("${CPP_STANDARD}" STREQUAL "C++20")
    set(CMAKE_CXX_STANDARD 20)
else()
    message("CPP_STANDARD NOT UNDERSTOOD; USING C++20 STANDARD")
    set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS Off)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

FetchContent_Declare(
    ConcurrentQueue
    GIT_REPOSITORY https://github.com/cameron314/concurrentqueue.git
    GIT_TAG master
    GIT_SHALLOW TRUE
    GIT_PROGRESS FALSE
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
)
FetchContent_GetProperties(ConcurrentQueue)

if(NOT ConcurrentQueue_POPULATED)
  FetchContent_Populate(ConcurrentQueue)
  add_subdirectory(${concurrentqueue_SOURCE_DIR} ${concurrentqueue_BINARY_DIR})
  add_library(ConcurrentQueue INTERFACE)
  target_include_directories(ConcurrentQueue INTERFACE ${concurrentqueue_SOURCE_DIR})
endif()

if(WITH_BOOST_COROUTINES OR WITH_TRACER_SUPPORT)
  find_package(Boost 1.74.0 COMPONENTS coroutine context)
  if(Boost_FOUND)
    message("Boost componentis 'context' and 'coroutine' found on system.")
  else()
    message("Boost components required NOT found on system; fetching from GitHub.")
    set(BOOST_INCLUDE_LIBRARIES context)
    set(BOOST_ENABLE_CMAKE ON)
    FetchContent_Declare(
        Boost
        URL https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.7z
        USES_TERMINAL_DOWNLOAD TRUE
        DOWNLOAD_NO_EXTRACT FALSE
    )
    FetchContent_MakeAvailable(Boost)
  endif()
endif()

set(messaging_HEADERS
    include/messaging/multithread/all.hpp
    include/messaging/multithread/message_queue.hpp
    include/messaging/multithread/types.hpp

    include/messaging/async.hpp
    include/messaging/broadcasting_process.hpp
    include/messaging/close_queue.hpp
    include/messaging/dispatcher_base.hpp
    include/messaging/dispatcher.hpp
    include/messaging/naming.hpp
    include/messaging/process_manager.hpp
    include/messaging/process.hpp
    include/messaging/pump.hpp
    include/messaging/queue_base.hpp
    include/messaging/receiver.hpp
    include/messaging/sender.hpp
    include/messaging/stop_source.hpp
    include/messaging/stop_token.hpp
    include/messaging/template_dispatcher.hpp
    include/messaging/time.hpp

    include/util/algorithm.hpp
    include/util/allocator.hpp
    include/util/attributes.hpp
    include/util/bundle.hpp
    include/util/chrono_io.hpp
    include/util/logger.hpp
    include/util/inspect.hpp
    include/util/lock_free_queue.hpp
    include/util/math.hpp
    include/util/pmte.hpp
    include/util/pointer_cast.hpp
    include/util/shared_library.hpp
    include/util/simulated_clock.hpp
    include/util/traits.hpp
    include/util/type_info.hpp
    include/util/type_list.hpp
)

if(WITH_BOOST_COROUTINES)
list(APPEND messaging_HEADERS
    include/messaging/coroutine/all.hpp
    include/messaging/coroutine/message_queue.hpp
    include/messaging/coroutine/types.hpp
)
endif()

set(messaging_SOURCES
    src/util__allocator.cpp
    src/util__logger.cpp
    src/util__pmte.cpp
    src/util__simulated_clock.cpp
    src/util__type_info.cpp
    src/messaging__process.cpp
)

add_library(${PROJECT_NAME} SHARED ${messaging_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET default)
target_include_directories(${PROJECT_NAME} PRIVATE ${concurrentqueue_SOURCE_DIR})

if(WITH_GCOV)
  set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage -fno-inline")
  set(CMAKE_EXE_LINKER_FLAGS "--coverage")
  set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
endif()

# Link Time Optimization
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported_result)
if(ipo_supported_result)
  set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# -march flag?
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if(COMPILER_SUPPORTS_MARCH_NATIVE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()

# Tracy
if(WITH_TRACY)
    add_compile_definitions(WITH_TRACY)
    if(TRACY_ON_VIRTUAL_MACHINE)
        add_definitions(-DTRACY_TIMER_FALLBACK)
    endif()
    FetchContent_Declare(
        Tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
        GIT_TAG v0.8.2.1
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    FetchContent_GetProperties(Tracy)
    if(NOT tracy_POPULATED)
      FetchContent_Populate(tracy)
      add_subdirectory(${tracy_SOURCE_DIR} ${tracy_BINARY_DIR})
    endif()
    target_include_directories(${PROJECT_NAME} PRIVATE ${tracy_SOURCE_DIR})
    target_link_libraries(${PROJECT_NAME} PUBLIC TracyClient.a)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC ConcurrentQueue pthread)

if(WITH_BOOST_COROUTINES)
    if(NOT Boost_FOUND)
        target_include_directories(${PROJECT_NAME} PUBLIC ${boost_SOURCE_DIR}/libs/coroutine2/include)
    endif()
    target_link_libraries(${PROJECT_NAME} PUBLIC Boost::context)
    add_compile_definitions(WITH_BOOST_COROUTINES)
endif()

if(WITH_TRACER_SUPPORT)
    if(NOT Boost_FOUND)
        target_include_directories(${PROJECT_NAME} PUBLIC ${boost_SOURCE_DIR}/libs/preprocessor/include)
    endif()
    add_compile_definitions(WITH_TRACER_SUPPORT)
endif()

if(WITH_MESSAGE_SET_THREAD_NAME)
    add_compile_definitions(WITH_MESSAGE_SET_THREAD_NAME)
endif()

if(WITH_MESSAGE_SET_THREAD_PRIORITY)
    add_compile_definitions(WITH_MESSAGE_SET_THREAD_PRIORITY)
endif()

if(WITH_MESSAGE_SET_THREAD_AFFINITY)
    add_compile_definitions(WITH_MESSAGE_SET_THREAD_AFFINITY)
endif()

if(WITH_TRACY)
    target_link_directories(${PROJECT_NAME} PRIVATE ${tracy_BINARY_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE TracyClient)
endif()

if(WITH_GCOV)
    add_compile_definitions(WITH_GCOV)
endif()
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/" # source directory
        DESTINATION "include" # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*" # select header files
)

if(WITH_EXAMPLES)
    add_subdirectory(examples)
endif()

if(WITH_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

