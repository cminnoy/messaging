include(GoogleTest)
find_package(GTest)

if(WITH_GCOV)
  set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS "--coverage")
  set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
endif()

link_directories(${CMAKE_BINARY_DIR})
add_definitions(-DALLOW_CHANGE_THREAD_NAME)

# Link Time Optimization
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported_result)

# -march flag?
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if(COMPILER_SUPPORTS_MARCH_NATIVE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -fwhole-program")
endif()

function(add_cpp_test TESTNAME)
    add_executable(test_${TESTNAME} ${ARGN})
    set_target_properties(test_${TESTNAME} PROPERTIES CXX_VISIBILITY_PRESET default)
    target_link_libraries(test_${TESTNAME} PRIVATE messaging GTest::GTest GTest::Main)
    target_include_directories(test_${TESTNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../include)

    if(WITH_TRACY)
      target_link_directories(test_${TESTNAME} PRIVATE ${tracy_BINARY_DIR})
      target_link_libraries(test_${TESTNAME} PRIVATE TracyClient)
    endif()

    if(ipo_supported_result)
      set_property(TARGET test_${TESTNAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()

    message("Add test " ${PROOT})
    add_test(NAME ${TESTNAME} COMMAND ${PROOT} $<TARGET_FILE:test_${TESTNAME}>)
endfunction()

add_cpp_test(simulated_clock test_simulated_clock.cpp)
add_cpp_test(bundle test_bundle.cpp)
add_cpp_test(type_info test_type_info.cpp)
add_cpp_test(pmte test_pmte.cpp)
add_cpp_test(pmte_traits test_pmte_traits.cpp)
add_cpp_test(pointer_cast test_pointer_cast.cpp)
add_cpp_test(handler test_handler.cpp)
add_cpp_test(visitor test_visitor.cpp)
add_cpp_test(messaging test_messaging.cpp)
add_cpp_test(messaging_perf test_messaging_perf.cpp)
