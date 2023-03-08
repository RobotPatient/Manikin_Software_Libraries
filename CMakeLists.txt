cmake_minimum_required(VERSION 3.8)

# Enable CMake unit-testing
enable_testing()

# Automatically fetch googletest (including gmock)
include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Automatically find all submodules in the modules folder and add them to the build
macro(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        SET(dirlist ${dirlist} ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
endmacro()

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR}/modules)

FOREACH(subdir ${SUBDIRS})
IF(EXISTS ${CMAKE_CURRENT_LIST_DIR}/modules/${subdir}/test)
  MESSAGE(STATUS "FOUND MODULE UNIT TEST:" ${subdir}/test)
  ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/${subdir}/test)
ELSE()
  ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/${subdir})
ENDIF()
ENDFOREACH()

