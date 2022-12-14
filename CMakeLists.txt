CMAKE_MINIMUM_REQUIRED( VERSION 3.11.0 )
PROJECT(cxx-config)

# Use GNUInstallDirs for Unix predefined directories
INCLUDE(GNUInstallDirs)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL "" FORCE)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL "" FORCE)

#####################################
# Options
#####################################

OPTION(BUILD_SHARED_LIBS "Build package with shared libraries." ON)
IF(NOT BUILD_SHARED_LIBS)
	SET(CMAKE_EXE_LINKER_FLAGS "-static")
	SET(LINK_SEARCH_START_STATIC TRUE)
ELSE()
	MESSAGE(STATUS "Shared libraries enabled")
ENDIF(NOT BUILD_SHARED_LIBS)


# C++17
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)


IF (BUILD_SHARED_LIBS AND CMAKE_SIZEOF_VOID_P EQUAL 8) # -fPIC is only required
	SET( CMAKE_CXX_FLAGS "-fPIC ${CMAKE_CXX_FLAGS}" )
	SET( CMAKE_C_FLAGS   "-fPIC ${CMAKE_C_FLAGS}")
	SET (CMAKE_POSITION_INDEPENDENT_CODE TRUE)
ELSE()
	SET (CMAKE_POSITION_INDEPENDENT_CODE FALSE)
ENDIF()


FILE(GLOB HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

ADD_LIBRARY(cxx-config INTERFACE )

# Required c++ features - use to determine what C++ version is required
TARGET_COMPILE_FEATURES(cxx-config INTERFACE cxx_constexpr cxx_noexcept cxx_override
	cxx_sizeof_member cxx_static_assert cxx_decltype cxx_defaulted_functions cxx_relaxed_constexpr
	cxx_default_function_template_args)

TARGET_INCLUDE_DIRECTORIES(cxx-config INTERFACE
											$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
											$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
											$<INSTALL_INTERFACE:src>)

INSTALL(TARGETS cxx-config DESTINATION lib)

OPTION(CXXCONFIG_BUILD_WTIH_TEST "Build and enabled tests" OFF)
IF(CXXCONFIG_BUILD_WTIH_TEST)
	ENABLE_TESTING()
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/test)
ENDIF()