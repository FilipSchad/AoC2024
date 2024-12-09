cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME day1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE PROJECT_SOURCES "*.cpp" "*.h")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# Set postfix for DEBUG
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

#Set the output dir
set(output_subdir "${CMAKE_BINARY_DIR}/bin")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${output_subdir})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${output_subdir})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${output_subdir})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${output_subdir})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${output_subdir})
	
if (MSVC)
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
