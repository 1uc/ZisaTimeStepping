cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(ZisaTimeStepping VERSION 0.1)

include("cmake/has_build_type.cmake")
include("cmake/warning_flags.cmake")
include("cmake/fast_debug.cmake")
include("cmake/sanitizers.cmake")
include("cmake/gperftools.cmake")
include("cmake/clang_tidy.cmake")

# -- Dependencies --------------------------------------------------------------
add_library(time_stepping_dependencies INTERFACE)

# MPI
if(ZISA_HAS_MPI)
  find_package(MPI REQUIRED COMPONENTS CXX)
  target_compile_definitions(time_stepping_dependencies INTERFACE ZISA_HAS_MPI=1)
  target_link_libraries(time_stepping_dependencies INTERFACE MPI::MPI_CXX)
endif()

# experimental/filesystem
target_link_libraries(time_stepping_dependencies INTERFACE stdc++fs)

# Logging
if(ZISA_LOG_LEVEL)
  target_compile_definitions(time_stepping_dependencies INTERFACE ZISA_LOG_LEVEL=${ZISA_LOG_LEVEL})
endif()

# -- Internal dependencies -----------------------------------------------------
find_package(ZisaCore REQUIRED)
target_link_libraries(time_stepping_dependencies INTERFACE Zisa::core)

find_package(ZisaMemory REQUIRED)
target_link_libraries(time_stepping_dependencies INTERFACE Zisa::memory)

if(ZISA_HAS_MPI)
  find_package(ZisaMPI REQUIRED)
  target_link_libraries(time_stepping_dependencies INTERFACE Zisa::mpi)
endif()


# -- Zisa ----------------------------------------------------------------------
# -- time_stepping -------------------------------------------------------------
add_library(time_stepping STATIC "")
set_target_properties(time_stepping PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(time_stepping PROPERTIES OUTPUT_NAME zisa_time_stepping)
set_target_properties(time_stepping PROPERTIES CXX_STANDARD 17)
target_include_directories(time_stepping PUBLIC
  $<BUILD_INTERFACE:${ZisaTimeStepping_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(time_stepping PUBLIC time_stepping_dependencies)
target_link_libraries(time_stepping PRIVATE time_stepping_warning_flags)

# -- clang tidy ----------------------------------------------------------------
if(CLANG_TIDY_EXE)
  set_target_properties(time_stepping PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
endif()

# -- install -------------------------------------------------------------------
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/ZisaTimeSteppingConfigVersion.cmake"
  VERSION ${ZisaTimeStepping_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
  "cmake/ZisaTimeSteppingConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/ZisaTimeSteppingConfigVersion.cmake"
  DESTINATION lib/cmake/zisa
)

add_library(Zisa::time_stepping ALIAS time_stepping)
install(
  TARGETS time_stepping
          time_stepping_dependencies
          time_stepping_warning_flags

  EXPORT ZisaTimeSteppingTargets
)
include(GNUInstallDirs)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ZisaTimeSteppingTargets
  FILE ZisaTimeSteppingTargets.cmake
  NAMESPACE Zisa::
  DESTINATION lib/cmake/zisa
)


# -- Tests ---------------------------------------------------------------------
find_package(Catch2 REQUIRED)

add_executable(time_stepping_unit_tests "")
set_target_properties(time_stepping_unit_tests PROPERTIES OUTPUT_NAME unit_tests)
set_target_properties(time_stepping_unit_tests PROPERTIES CXX_STANDARD 17)
target_include_directories(time_stepping_unit_tests PUBLIC test)
target_link_libraries(time_stepping_unit_tests PUBLIC Zisa::time_stepping Catch2::Catch2)

enable_testing()
add_test(NAME UnitTests COMMAND time_stepping_unit_tests)

# -- Subdirectories ------------------------------------------------------------
add_subdirectory(src)
add_subdirectory(test)
