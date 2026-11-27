# Unit tests use the header-only variant of Boost.Test, so only the Boost headers are needed.
# If they are not found, set BOOST_ROOT (CMake variable or environment variable).
find_path(BOOST_TEST_INCLUDE_DIR boost/test/included/unit_test.hpp
  HINTS "${BOOST_ROOT}" "$ENV{BOOST_ROOT}"
  PATH_SUFFIXES include)
if(NOT BOOST_TEST_INCLUDE_DIR)
  message(FATAL_ERROR
    "Boost headers not found. Install them (Ubuntu: libboost-test-dev) or set BOOST_ROOT.\n"
    "To build without tests, configure with -DBUILD_TESTING=OFF.")
endif()

# add_plugin_test(<plugin name> <sources...>)
function(add_plugin_test name)
  add_executable(${name}.test ${ARGN})
  target_include_directories(${name}.test PRIVATE "${BOOST_TEST_INCLUDE_DIR}")
  add_test(NAME ${name} COMMAND ${name}.test)
endfunction()
