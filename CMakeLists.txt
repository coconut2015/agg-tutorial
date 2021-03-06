CMAKE_MINIMUM_REQUIRED (VERSION 3.1)
project (agg-tutorial
	VERSION 0.0.0.1
	LANGUAGES CXX
)

set( CMAKE_SYSTEM_NAME Linux )
set( CMAKE_CXX_STANDARD 11 )
enable_testing()

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)

add_custom_target(ctest COMMAND ${CMAKE_CTEST_COMMAND})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

#
# Setup Code Coverage
#
if(COVERAGE)
	include(CodeCoverage)

	setup_target_for_coverage_lcov (
		NAME test_coverage
		EXECUTABLE ctest
		DEPENDENCIES ctest
	)

	APPEND_COVERAGE_COMPILER_FLAGS()
endif(COVERAGE)

#
# Setup Doxygen
#
if(BUILD_DOC)
	# check if Doxygen is installed
	find_package(Doxygen)
	if (DOXYGEN_FOUND)
		# set input and output files
		set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/Doxyfile.in)
		set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

		# request to configure the file
		configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
		message("Doxygen build started")

		# note the option ALL which allows to build the docs together with the application
		add_custom_target( doc_doxygen ALL
			COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating API documentation with Doxygen"
			VERBATIM )
	else (DOXYGEN_FOUND)
		message("Doxygen need to be installed to generate the doxygen documentation")
	endif (DOXYGEN_FOUND)
endif(BUILD_DOC)

include(agg)

include_directories(
	tutorial
	agg-2.4/include
)

link_libraries(
	png
	agg
)

# -- tutorials ---------------------------------------------------------------

# .. tutorial_rendererbase
add_executable(tutorial_rendererbase
	tutorial/tutorial_rendererbase.cpp)
add_test(NAME tutorial_rendererbase
    COMMAND tutorial_rendererbase ../images)

add_executable(tutorial_path_1
	tutorial/tutorial_path_1.cpp)
add_test(NAME tutorial_path_1
    COMMAND tutorial_path_1  ../images)

add_executable(tutorial_path_2
	tutorial/tutorial_path_2.cpp)
add_test(NAME tutorial_path_2
    COMMAND tutorial_path_2 ../images)

add_executable(tutorial_path_3
	tutorial/tutorial_path_3.cpp)
add_test(NAME tutorial_path_3
    COMMAND tutorial_path_3 ../images)

add_executable(tutorial_linedrawing_1
	tutorial/tutorial_linedrawing_1.cpp)
add_test(NAME tutorial_linedrawing_1
    COMMAND tutorial_linedrawing_1  ../images)

add_executable(tutorial_linedrawing_2
	tutorial/tutorial_linedrawing_2.cpp)
add_test(NAME tutorial_linedrawing_2
    COMMAND tutorial_linedrawing_2  ../images)

add_executable(tutorial_linedrawing_3
	tutorial/tutorial_linedrawing_3.cpp)
add_test(NAME tutorial_linedrawing_3
    COMMAND tutorial_linedrawing_3  ../images)

add_executable(tutorial_linedrawing_4
	tutorial/tutorial_linedrawing_4.cpp)
add_test(NAME tutorial_linedrawing_4
    COMMAND tutorial_linedrawing_4  ../images)

add_executable(tutorial_image_1
	tutorial/tutorial_image_1.cpp)
add_test(NAME tutorial_image_1
    COMMAND tutorial_image_1  ../images)

add_executable(tutorial_image_2
	tutorial/tutorial_image_2.cpp)
add_test(NAME tutorial_image_2
    COMMAND tutorial_image_2  ../images)

add_executable(tutorial_font_1
	tutorial/tutorial_font_1.cpp)
add_test(NAME tutorial_font_1
    COMMAND tutorial_font_1  ../images)

add_executable(tutorial_font_2
	tutorial/tutorial_font_2.cpp)
add_test(NAME tutorial_font_2
    COMMAND tutorial_font_2  ../images)

add_executable(tutorial_gradient_1
	tutorial/tutorial_gradient_1.cpp)
add_test(NAME tutorial_gradient_1
    COMMAND tutorial_gradient_1  ../images)

add_executable(tutorial_layer
	tutorial/tutorial_layer.cpp)
add_test(NAME tutorial_layer
    COMMAND tutorial_layer  ../images)

add_executable(tutorial_blur
	tutorial/tutorial_blur.cpp)
add_test(NAME tutorial_blur
    COMMAND tutorial_blur  ../images)

add_executable(tutorial_alphamask
	tutorial/tutorial_alphamask.cpp)
add_test(NAME tutorial_alphamask
    COMMAND tutorial_alphamask  ../images)

add_executable(tutorial_shape_1
	tutorial/tutorial_shape_1.cpp)
add_test(NAME tutorial_shape_1
    COMMAND tutorial_shape_1  ../images)

add_executable(tutorial_shape_2
	tutorial/tutorial_shape_2.cpp)
add_test(NAME tutorial_shape_2
    COMMAND tutorial_shape_2  ../images)

# -- references --------------------------------------------------------------

add_executable(reference_linecap
	reference/reference_linecap.cpp)
add_test(NAME reference_linecap
    COMMAND reference_linecap  ../images)
