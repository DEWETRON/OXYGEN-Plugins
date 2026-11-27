# Sets up the ODK from the submodule in external/odk.
#
# Do not add external/odk itself: its top-level CMakeLists.txt only does its setup when it is
# the main project, and it always builds all SDK examples.

set(ODK_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/external/odk")

if(NOT EXISTS "${ODK_ROOT}/odk/CMakeLists.txt")
  message(FATAL_ERROR
    "The ODK submodule is missing. Run:\n"
    "  git submodule update --init\n"
    "(A ZIP download from GitHub does not contain it. Use git clone.)")
endif()

# Warn if the checked-out ODK is not the version this commit records.
find_package(Git QUIET)
if(Git_FOUND)
  execute_process(COMMAND "${GIT_EXECUTABLE}" submodule status -- external/odk
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VARIABLE odk_status ERROR_QUIET RESULT_VARIABLE odk_status_result)
  if(odk_status_result EQUAL 0 AND odk_status MATCHES "^\\+")
    message(WARNING
      "external/odk is not at the recorded version. Unless you are upgrading the ODK, run:\n"
      "  git submodule update --init")
  endif()
endif()

list(APPEND CMAKE_MODULE_PATH "${ODK_ROOT}/cmake")

# Same linking as the SDK: static runtime and libraries on Windows.
if(WIN32)
  set(3RDPARTY_LINK_TYPE "static")
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

include(CMakeSettings)
include(OxygenPluginFunctions)
SetCommonOutputDirectory()

# Provides qt5_add_binary_resources(), which packs the QML files. On Windows, SDK_QT_PATH can
# point to a folder with the Qt resource compiler. The ODK only reads the CMake variable.
if(NOT SDK_QT_PATH AND DEFINED ENV{SDK_QT_PATH})
  file(TO_CMAKE_PATH "$ENV{SDK_QT_PATH}" SDK_QT_PATH)
endif()
find_package(SDKQt REQUIRED)

AddUniqueTargetFromSubdirectory(pugixml "${ODK_ROOT}/3rdparty/pugixml-1.9/scripts" "3rdparty/pugixml-1.9")
add_subdirectory("${ODK_ROOT}/odk" odk)
