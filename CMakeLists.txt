cmake_minimum_required(VERSION 3.24)

project(aoc2024 CXX)

# Turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the version of C++ for the project
set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE PROJECT_SOURCES
    "src/*.h*"
    "src/*.c*"
    "include/*.h*"
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "include")

# Set postfix for DEBUG
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

#Set the output dir
set(output_subdir "${CMAKE_SOURCE_DIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${output_subdir})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${output_subdir})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${output_subdir})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${output_subdir})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${output_subdir})

# Nastaveni parametru kompilatoru
if (MSVC)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

    target_compile_options(${PROJECT_NAME} PUBLIC
        "/MP" "/W4" "/sdl" "/GR" "/DWIN64"
    "$<$<CONFIG:Debug>:/MDd>"
    "$<$<CONFIG:MinSizeRel>:/MD>"
    "$<$<CONFIG:MinSizeRel>:/GL>"
    "$<$<CONFIG:MinSizeRel>:/Oi>"
    "$<$<CONFIG:MinSizeRel>:/Gy>"
    "$<$<CONFIG:Release>:/MD>"
    "$<$<CONFIG:Release>:/GL>"
    "$<$<CONFIG:Release>:/Oi>"
    "$<$<CONFIG:Release>:/Gy>"
    "$<$<CONFIG:RelWithDebInfo>:/MD>"
    "$<$<CONFIG:RelWithDebInfo>:/GL>"
    "$<$<CONFIG:RelWithDebInfo>:/Oi>"
    "$<$<CONFIG:RelWithDebInfo>:/Gy>")

    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/MAP /LTCG:incremental /OPT:REF /OPT:ICF /SUBSYSTEM:CONSOLE")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_MINSIZEREL "/MAP /LTCG:incremental /OPT:REF /OPT:ICF /SUBSYSTEM:CONSOLE")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/MAP /LTCG:incremental /OPT:REF /OPT:ICF /SUBSYSTEM:CONSOLE")
else()
    target_compile_options(${PROJECT_NAME} PUBLIC "-Wall" "-Wextra" "-pedantic")
endif()
