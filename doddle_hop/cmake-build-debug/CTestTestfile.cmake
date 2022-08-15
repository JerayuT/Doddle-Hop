# CMake generated Testfile for 
# Source directory: /Users/mitch/Downloads/doddle_hop
# Build directory: /Users/mitch/Downloads/doddle_hop/cmake-build-debug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Test_model_test "model_test")
set_tests_properties(Test_model_test PROPERTIES  _BACKTRACE_TRIPLES "/Users/mitch/Downloads/doddle_hop/.cs211/cmake/211commands.cmake;90;add_test;/Users/mitch/Downloads/doddle_hop/CMakeLists.txt;25;add_test_program;/Users/mitch/Downloads/doddle_hop/CMakeLists.txt;0;")
subdirs(".cs211/lib/catch")
subdirs(".cs211/lib/ge211")
