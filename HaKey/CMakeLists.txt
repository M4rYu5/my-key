cmake_minimum_required (VERSION 3.30)


# Enable Hot Reload for MSVC compilers if supported.
#   fails when compiling from windows with clang
# if (POLICY CMP0141)
#   cmake_policy(SET CMP0141 NEW)
#   set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
# endif()

# --- SUPPOSED TO AUTOMATICALLY SEARCH FOR MODULES, BUT ONLY WORKS FOR LINUX  ---
    #set(CMAKE_CXX_COMPILER "clang++")
    #set(CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS "/usr/local/bin/clang-scan-deps")
    #set(CMAKE_CXX_SCAN_FOR_MODULES ON)
# --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


if (WIN32)
  add_compile_definitions(WINDOWS)
elseif(UNIX)
  add_compile_definitions(LINUX)
elseif(APPLE)
  add_compile_definitions(MACOS)
endif()



set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(HaKey LANGUAGES CXX)


# ADD FILES
file(GLOB F_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)
add_executable(HaKey ${F_SOURCES})

# ADD MODULES
file(GLOB_RECURSE M_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/key/*.cppm"
)
target_sources(HaKey
  PRIVATE
    FILE_SET all_my_modules TYPE CXX_MODULES FILES
    "${M_SOURCES}"
)