﻿# Makefile generated by CMakefileGenerator.cs (v1.2)
# *DO NOT EDIT*

cmake_minimum_required (VERSION 2.6)
project (UE4)

# CMake Flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS 1 CACHE BOOL "" FORCE)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 1 CACHE BOOL "" FORCE)

# Standard Includes
include("O:/Projects/Unreal Engine/Lutenium/Lutenium/Intermediate/ProjectFiles/cmake-includes.cmake")
include("O:/Projects/Unreal Engine/Lutenium/Lutenium/Intermediate/ProjectFiles/cmake-definitions.cmake")
include("O:/Projects/Unreal Engine/Lutenium/Lutenium/Intermediate/ProjectFiles/cmake-headers-ue4.cmake")
include("O:/Projects/Unreal Engine/Lutenium/Lutenium/Intermediate/ProjectFiles/cmake-headers-project.cmake")
include("O:/Projects/Unreal Engine/Lutenium/Lutenium/Intermediate/ProjectFiles/cmake-sources-engine.cmake")
include("O:/Projects/Unreal Engine/Lutenium/Lutenium/Intermediate/ProjectFiles/cmake-sources-project.cmake")
include("O:/Projects/Unreal Engine/Lutenium/Lutenium/Intermediate/ProjectFiles/cmake-csharp-engine.cmake")
include("O:/Projects/Unreal Engine/Lutenium/Lutenium/Intermediate/ProjectFiles/cmake-csharp-project.cmake")

# Optional Shader Include
include("O:/Projects/Unreal Engine/Lutenium/Lutenium/Intermediate/ProjectFiles/cmake-shaders-engine.cmake")
set_source_files_properties(${ENGINE_SHADER_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
include("O:/Projects/Unreal Engine/Lutenium/Lutenium/Intermediate/ProjectFiles/cmake-shaders-project.cmake")
set_source_files_properties(${PROJECT_SHADER_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("Shader Files" REGULAR_EXPRESSION .*.usf)

# Optional Config Include
include("O:/Projects/Unreal Engine/Lutenium/Lutenium/Intermediate/ProjectFiles/cmake-config-engine.cmake")
set_source_files_properties(${ENGINE_CONFIG_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
include("O:/Projects/Unreal Engine/Lutenium/Lutenium/Intermediate/ProjectFiles/cmake-config-project.cmake")
set_source_files_properties(${PROJECT_CONFIG_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("Config Files" REGULAR_EXPRESSION .*.ini)

add_custom_target(UE4Editor-Win64-DebugGame call "O:/Programs/Unreal Engine/UE_4.24/Engine/Build/BatchFiles/Build.bat" UE4Editor Win64 DebugGame  -game -progress -buildscw VERBATIM)
add_custom_target(UE4Editor-Win64-Shipping call "O:/Programs/Unreal Engine/UE_4.24/Engine/Build/BatchFiles/Build.bat" UE4Editor Win64 Shipping  -game -progress -buildscw VERBATIM)
add_custom_target(UE4Editor call "O:/Programs/Unreal Engine/UE_4.24/Engine/Build/BatchFiles/Build.bat" UE4Editor Win64 Development  -game -progress -buildscw VERBATIM)

add_custom_target(UE4Game-Win64-DebugGame call "O:/Programs/Unreal Engine/UE_4.24/Engine/Build/BatchFiles/Build.bat" UE4Game Win64 DebugGame  -game -progress -buildscw VERBATIM)
add_custom_target(UE4Game-Win64-Shipping call "O:/Programs/Unreal Engine/UE_4.24/Engine/Build/BatchFiles/Build.bat" UE4Game Win64 Shipping  -game -progress -buildscw VERBATIM)
add_custom_target(UE4Game call "O:/Programs/Unreal Engine/UE_4.24/Engine/Build/BatchFiles/Build.bat" UE4Game Win64 Development  -game -progress -buildscw VERBATIM)

add_custom_target(Lutenium-Win64-DebugGame call "O:/Programs/Unreal Engine/UE_4.24/Engine/Build/BatchFiles/Build.bat" Lutenium Win64 DebugGame "-project=O:/Projects/Unreal Engine/Lutenium/Lutenium/Lutenium.uproject" -game -progress -buildscw VERBATIM)
add_custom_target(Lutenium-Win64-Shipping call "O:/Programs/Unreal Engine/UE_4.24/Engine/Build/BatchFiles/Build.bat" Lutenium Win64 Shipping "-project=O:/Projects/Unreal Engine/Lutenium/Lutenium/Lutenium.uproject" -game -progress -buildscw VERBATIM)
add_custom_target(Lutenium call "O:/Programs/Unreal Engine/UE_4.24/Engine/Build/BatchFiles/Build.bat" Lutenium Win64 Development "-project=O:/Projects/Unreal Engine/Lutenium/Lutenium/Lutenium.uproject" -game -progress -buildscw VERBATIM)

add_custom_target(LuteniumEditor-Win64-DebugGame call "O:/Programs/Unreal Engine/UE_4.24/Engine/Build/BatchFiles/Build.bat" LuteniumEditor Win64 DebugGame "-project=O:/Projects/Unreal Engine/Lutenium/Lutenium/Lutenium.uproject" -game -progress -buildscw VERBATIM)
add_custom_target(LuteniumEditor-Win64-Shipping call "O:/Programs/Unreal Engine/UE_4.24/Engine/Build/BatchFiles/Build.bat" LuteniumEditor Win64 Shipping "-project=O:/Projects/Unreal Engine/Lutenium/Lutenium/Lutenium.uproject" -game -progress -buildscw VERBATIM)
add_custom_target(LuteniumEditor call "O:/Programs/Unreal Engine/UE_4.24/Engine/Build/BatchFiles/Build.bat" LuteniumEditor Win64 Development "-project=O:/Projects/Unreal Engine/Lutenium/Lutenium/Lutenium.uproject" -game -progress -buildscw VERBATIM)

add_executable(FakeTarget ${ENGINE_HEADER_FILES} ${ENGINE_SOURCE_FILES} ${ENGINE_CSHARP_FILES} ${ENGINE_SHADER_FILES} ${ENGINE_CONFIG_FILES} ${PROJECT_HEADER_FILES} ${PROJECT_SOURCE_FILES} ${PROJECT_CSHARP_FILES} ${PROJECT_SHADER_FILES} ${PROJECT_CONFIG_FILES})
