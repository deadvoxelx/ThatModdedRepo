set(LUA54_DIR "${CMAKE_SOURCE_DIR}/third_party/lua54")

if(NOT EXISTS "${LUA54_DIR}/lua.h")
  message(FATAL_ERROR "Vendored Lua 5.4 sources not found at ${LUA54_DIR}")
endif()

file(GLOB LUA54_SOURCES "${LUA54_DIR}/*.c")
list(REMOVE_ITEM LUA54_SOURCES "${LUA54_DIR}/lua.c" "${LUA54_DIR}/luac.c")

add_library(lua54 STATIC ${LUA54_SOURCES})
target_include_directories(lua54 PUBLIC "${LUA54_DIR}")
set_target_properties(lua54 PROPERTIES LINKER_LANGUAGE C)
set_property(TARGET lua54 PROPERTY FOLDER "Ruby Launcher")

if(MSVC)
  target_compile_options(lua54 PRIVATE /W0)
endif()

file(GLOB_RECURSE RUBY_LAUNCHER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(Ruby.Launcher STATIC ${RUBY_LAUNCHER_SOURCES})

target_include_directories(Ruby.Launcher PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/includes"
)

target_include_directories(Ruby.Launcher PRIVATE
  "${CMAKE_SOURCE_DIR}/Minecraft.Client"
  "${CMAKE_SOURCE_DIR}/Minecraft.Client/${PLATFORM_NAME}/Iggy/include"
  "${CMAKE_SOURCE_DIR}/Minecraft.World"
  "${CMAKE_SOURCE_DIR}/include/"
  "${CMAKE_BINARY_DIR}/generated/"
)

set(RUBY_LAUNCHER_DEFINES ${MINECRAFT_SHARED_DEFINES})
list(FILTER RUBY_LAUNCHER_DEFINES EXCLUDE REGEX "_HAS_STD_BYTE")
target_compile_definitions(Ruby.Launcher PRIVATE ${RUBY_LAUNCHER_DEFINES} PUBLIC RUBY_LAUNCHER)

if(COMMAND configure_compiler_target)
  configure_compiler_target(Ruby.Launcher)
endif()

if(MSVC)
  target_compile_options(Ruby.Launcher PRIVATE /bigobj)
endif()

target_precompile_headers(Ruby.Launcher PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_SOURCE_DIR}/Minecraft.Client/stdafx.h>")

target_link_libraries(Ruby.Launcher PUBLIC lua54 PRIVATE Minecraft.World)

set_property(TARGET Ruby.Launcher PROPERTY FOLDER "Ruby Launcher")
