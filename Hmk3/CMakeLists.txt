cmake_minimum_required(VERSION 3.18)
project(Hmk3_Skeleton LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin")

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")

add_executable(Hmk3_Skeleton ${SOURCES})

target_include_directories(Hmk3_Skeleton PRIVATE ${CMAKE_SOURCE_DIR}/src)

include_directories(
    ../external/glfw-3.1.2/include/
    ../external/glm-0.9.7.1/
    ../external/glew-1.13.0/include/
    ../external/assimp-3.0.1270/include/
    .
)


add_compile_definitions(
    TW_STATIC
    TW_NO_LIB_PRAGMA
    TW_NO_DIRECT3D
    GLEW_STATIC
    _CRT_SECURE_NO_WARNINGS
)
set(ALL_LIBS
    OpenGL::GL
    glfw
    GLEW_1130
    assimp
)

target_link_libraries(Hmk3_Skeleton PRIVATE
    ${ALL_LIBS}
)

if(UNIX AND NOT APPLE)
    target_link_libraries(Hmk3_Skeleton PRIVATE dl pthread)
endif()

# --- Copy assets folder to build directory ---

set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/assets")
set(DEST_DIR   "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets")

# Glob all files recursively
file(GLOB_RECURSE ASSET_FILES
    CONFIGURE_DEPENDS
    "${SOURCE_DIR}/*"
)
set_target_properties(Hmk3_Skeleton PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

# Create a custom target that copies files on every build
add_custom_target(copy_assets ALL
    COMMENT "Copying assets to build directory..."
)

foreach(SRC_FILE ${ASSET_FILES})
    # Compute the file's path relative to SOURCE_DIR
    file(RELATIVE_PATH REL_PATH "${SOURCE_DIR}" "${SRC_FILE}")

    set(DST_FILE "${DEST_DIR}/${REL_PATH}")

    # Get the destination subfolder so we can create it if needed
    get_filename_component(DST_DIR "${DST_FILE}" DIRECTORY)

    add_custom_command(
        TARGET copy_assets PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${DST_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SRC_FILE}" "${DST_FILE}"
        COMMENT "Copying ${REL_PATH}"
    )
endforeach()


# Make your executable depend on the copy step
add_dependencies(Hmk3_Skeleton copy_assets)