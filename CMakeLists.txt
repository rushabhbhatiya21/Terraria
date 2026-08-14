cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

#! ! ! ! ! ! !
#set this to ON instead of OFF to ship the game!
#basically this will change the RESOURCES_PATH to be the local path
#! ! ! ! ! ! !
#delete the out folder after changing if visual studio doesn recognize the change
option(PRODUCTION_BUILD "Make this a production build" OFF)

if(MSVC) 
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
add_compile_options(/arch:AVX2) #make sure SIMD optimizations take place
else()
	add_compile_options(-mavx2 -mfma) # make sure SIMD optimizations take place, but on linux and other platforms
endif()

#Ensure raylib is built as static libraries
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build Raylib as static libraries" FORCE)


project(mygame)

#add raylib and other third party libs
add_subdirectory(thirdParty/raylib-5.0)
add_subdirectory(thirdParty/imgui-docking)
add_subdirectory(thirdParty/rlImGui)
add_subdirectory(thirdParty/fastNoiseSIMD)
add_subdirectory(thirdParty/json-3.12.0)
add_library(glad thirdParty/glad/src/gl.c)


# MY_SOURCES is defined to be a list of all the source files for my game 
# DON'T ADD THE SOURCES BY HAND, they are already added with this macro
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable("${CMAKE_PROJECT_NAME}")

set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 17)
set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES OUTPUT_NAME "Terraframe")



if(PRODUCTION_BUILD)
	# setup the ASSETS_PATH macro to be in the root folder of your exe
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/") 

	# remove the option to debug asserts.
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1) 

else()
	# This is useful to get an ASSETS_PATH in your IDE during development
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0) 

endif()

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES} )


if(MSVC) # If using the VS compiler...

	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)

	if(PRODUCTION_BUILD)
		#YOU CAN REMOVE THE CONSOLE WITH THIS LINE!
		set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") #no console
	endif()

elseif(MINGW) # Windows / MinGW GCC/Clang
	if(PRODUCTION_BUILD)
		# Remove console on Windows under MinGW
		target_link_options("${CMAKE_PROJECT_NAME}" PRIVATE
			-mwindows
		)
	endif()

elseif(APPLE OR UNIX) # macOS / Linux
	if(PRODUCTION_BUILD)
		target_link_options("${CMAKE_PROJECT_NAME}" PRIVATE -s)
	endif()
endif()


target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/gameLayer")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/platform")

target_include_directories(glad
    PUBLIC
        thirdParty/glad/include
)

target_include_directories("${CMAKE_PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/freetype/include")

# Link raylib library statically
target_link_libraries("${CMAKE_PROJECT_NAME}"
    PRIVATE
        raylib_static
        imgui
        rlImGui
        fastNoiseSIMD
        nlohmann_json
        glad
		"${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/freetype/lib/freetype.lib"
)

# Production package (zip) for GitHub Releases.
# Use this only with -DPRODUCTION_BUILD=ON so packaged resources resolve correctly.
if(PRODUCTION_BUILD)
	set(RELEASE_VERSION "0.1.0")
	set(RELEASE_STAGE_DIR "${CMAKE_BINARY_DIR}/release/Terraframe")
	set(RELEASE_ZIP_PATH "${CMAKE_BINARY_DIR}/Terraframe-${RELEASE_VERSION}-windows.zip")

	add_custom_target(release_zip
		COMMAND ${CMAKE_COMMAND} -E rm -rf "${RELEASE_STAGE_DIR}"
		COMMAND ${CMAKE_COMMAND} -E make_directory "${RELEASE_STAGE_DIR}"
		COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${CMAKE_PROJECT_NAME}>" "${RELEASE_STAGE_DIR}/Terraframe.exe"
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/resources" "${RELEASE_STAGE_DIR}/resources"
		COMMAND ${CMAKE_COMMAND} -E rm -f "${RELEASE_ZIP_PATH}"
		COMMAND ${CMAKE_COMMAND} -E tar cfv "${RELEASE_ZIP_PATH}" --format=zip Terraframe
		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/release"
		DEPENDS "${CMAKE_PROJECT_NAME}"
		COMMENT "Creating release zip: ${RELEASE_ZIP_PATH}"
	)
endif()