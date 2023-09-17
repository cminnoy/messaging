#!/bin/sh

mkdir -p build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DWITH_TRACER_SUPPORT=ON -DWITH_MESSAGE_SET_THREAD_NAME=ON -DWITH_MESSAGE_SET_THREAD_PRIORITY=ON -DWITH_MESSAGE_SET_THREAD_AFFINITY=ON -DWITH_TRACY=OFF -DWITH_GCOV=ON -DWITH_TESTS=ON
cd build
make VERBOSE=1 -j
cd ..

# create initial info file
lcov -b . -c -i -d ./build -o coverage.init --rc lcov_branch_coverage=1 --no-external

# test simulated_clock
ctest --test-dir ./build -R simulated_clock
lcov -b . -c -d ./build -o coverage_simulated_clock.run --no-external --rc lcov_branch_coverage=1 -t simulated_clock
find build -name *.gcda | xargs rm

# test bundle
ctest --test-dir ./build -R bundle
lcov -b . -c -d ./build -o coverage_bundle.run --no-external --rc lcov_branch_coverage=1 -t bundle
find build -name *.gcda | xargs rm

# test type_info
ctest --test-dir ./build -R type_info
lcov -b . -c -d ./build -o coverage_type_info.run --no-external --rc lcov_branch_coverage=1 -t type_info
find build -name *.gcda | xargs rm

# test pmte_traits
ctest --test-dir ./build -R pmte_traits
lcov -b . -c -d ./build -o coverage_pmte_traits.run --no-external --rc lcov_branch_coverage=1 -t pmte_traits
find build -name *.gcda | xargs rm

# test pmte
ctest --test-dir ./build -R pmte
lcov -b . -c -d ./build -o coverage_pmte.run --no-external --rc lcov_branch_coverage=1 -t pmte
find build -name *.gcda | xargs rm

# test pointer_cast
ctest --test-dir ./build -R pointer_cast
lcov -b . -c -d ./build -o coverage_pointer_cast.run --no-external --rc lcov_branch_coverage=1 -t pointer_cast
find build -name *.gcda | xargs rm

# test handler
ctest --test-dir ./build -R handler
lcov -b . -c -d ./build -o coverage_handler.run --no-external --rc lcov_branch_coverage=1 -t handler
find build -name *.gcda | xargs rm

# test visitor
ctest --test-dir ./build -R visitor
lcov -b . -c -d ./build -o coverage_visitor.run --no-external --rc lcov_branch_coverage=1 -t visitor
find build -name *.gcda | xargs rm

# test messaging
ctest --test-dir ./build -R messaging -E messaging_
lcov -b . -c -d ./build -o coverage_messaging.run --no-external --rc lcov_branch_coverage=1 -t messaging
find build -name *.gcda | xargs rm

# test messaging_perf
ctest --test-dir ./build -R messaging_perf
lcov -b . -c -d ./build -o coverage_messaging_perf.run --no-external --rc lcov_branch_coverage=1 -t messaging_perf
find build -name *.gcda | xargs rm

# merge info files
lcov -b . -a coverage.init \
          -a coverage_simulated_clock.run \
          -a coverage_bundle.run \
          -a coverage_type_info.run \
          -a coverage_pmte_traits.run \
          -a coverage_pmte.run \
          -a coverage_pointer_cast.run \
          -a coverage_handler.run \
          -a coverage_visitor.run \
          -a coverage_messaging.run \
          -a coverage_messaging_perf.run \
          -o coverage.total \
          --rc lcov_branch_coverage=1

# remove unneeded profiling data
lcov  --remove coverage.total -o coverage.final \
      '/usr/include/*' \
      '/usr/lib/*' \
      '/usr/local/include/*' \
      "$PWD/build/*" \
      "$PWD/examples/*" \
      "$PWD/tests/*" \
      --rc lcov_branch_coverage=1

# generate description file
gendesc test_descriptions.txt --output-filename test_descriptions.desc

# generate html
genhtml -p $PWD --rc branch_coverage=1 --branch-coverage --title "Messaging Library" --show-details --legend coverage.final --output-directory=coverage_result --description-file test_descriptions.desc --num-spaces=4 --demangle-cpp --sort

