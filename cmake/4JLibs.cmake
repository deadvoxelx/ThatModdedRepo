set(MINECRAFT_4JLIBS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/4JLibs")
set(MINECRAFT_4JLIBS_HEADER_DIR "${CMAKE_SOURCE_DIR}/Minecraft.Client/Windows64/4JLibs/inc")

set(_4jlibs_can_build_from_source FALSE)
if(PLATFORM_NAME STREQUAL "Windows64" AND NOT CMAKE_CROSSCOMPILING)
  set(_4jlibs_can_build_from_source TRUE)
endif()

if(_4jlibs_can_build_from_source)
  set(_4jlibs_source_default ON)
else()
  set(_4jlibs_source_default OFF)
endif()

option(MINECRAFT_4JLIBS_FROM_SOURCE
  "Build the 4J platform libraries from ThirdParty/4JLibs instead of linking the prebuilt .lib files"
  ${_4jlibs_source_default}
)

set(MINECRAFT_4JLIBS_SOURCE_BUILD FALSE)

if(MINECRAFT_4JLIBS_FROM_SOURCE AND NOT _4jlibs_can_build_from_source)
  message(WARNING
    "4JLibs: MINECRAFT_4JLIBS_FROM_SOURCE is ON but the source build is only supported for native Windows64 builds. "
    "Falling back to the prebuilt libraries.")
endif()

if(_4jlibs_can_build_from_source AND MINECRAFT_4JLIBS_FROM_SOURCE)
  set(MINECRAFT_4JLIBS_SOURCE_BUILD TRUE)

  set(4JLibs_SOURCE_DIR "${CMAKE_BINARY_DIR}/4JLibs")

  if(NOT FXC_COMPILER)
    file(GLOB _4jlibs_fxc "C:/Program Files (x86)/Windows Kits/10/bin/*/x64/fxc.exe")
    if(NOT _4jlibs_fxc)
      message(FATAL_ERROR
        "4JLibs: could not find fxc.exe. Install the Windows SDK, or configure with -DFXC_COMPILER=<path to fxc.exe>.")
    endif()
    list(SORT _4jlibs_fxc)
    list(GET _4jlibs_fxc -1 FXC_COMPILER)
    message(STATUS "4JLibs: compiling shaders with ${FXC_COMPILER}")
  endif()

  add_library(4JLibs.Globals INTERFACE)
  target_include_directories(4JLibs.Globals INTERFACE "${MINECRAFT_4JLIBS_HEADER_DIR}")

  set(_4jlibs_zlib_dir "${MINECRAFT_4JLIBS_SOURCE_DIR}/impls/Windows_Libs/Render/vendor/zlib")

  add_library(4JLibs.zlib STATIC
    "${_4jlibs_zlib_dir}/adler32.c"
    "${_4jlibs_zlib_dir}/compress.c"
    "${_4jlibs_zlib_dir}/crc32.c"
    "${_4jlibs_zlib_dir}/deflate.c"
    "${_4jlibs_zlib_dir}/infback.c"
    "${_4jlibs_zlib_dir}/inffast.c"
    "${_4jlibs_zlib_dir}/inflate.c"
    "${_4jlibs_zlib_dir}/inftrees.c"
    "${_4jlibs_zlib_dir}/trees.c"
    "${_4jlibs_zlib_dir}/uncompr.c"
    "${_4jlibs_zlib_dir}/zutil.c"
  )
  target_include_directories(4JLibs.zlib PUBLIC "${_4jlibs_zlib_dir}")
  configure_compiler_target(4JLibs.zlib)
  set_target_properties(4JLibs.zlib PROPERTIES
    FOLDER "4JLibs"
    DEBUG_POSTFIX "_d"
  )

  add_subdirectory("${MINECRAFT_4JLIBS_SOURCE_DIR}/impls/Windows_Libs/Input" "${CMAKE_BINARY_DIR}/4JLibs/Input")
  add_subdirectory("${MINECRAFT_4JLIBS_SOURCE_DIR}/impls/Windows_Libs/Render" "${CMAKE_BINARY_DIR}/4JLibs/Render")
  add_subdirectory("${MINECRAFT_4JLIBS_SOURCE_DIR}/impls/Windows_Libs/Storage" "${CMAKE_BINARY_DIR}/4JLibs/Storage")

  foreach(_4jlibs_target 4JLibs.Windows64.Input 4JLibs.Windows64.Render 4JLibs.Windows64.Storage)
    configure_compiler_target(${_4jlibs_target})
    set_target_properties(${_4jlibs_target} PROPERTIES
      FOLDER "4JLibs"
      DEBUG_POSTFIX "_d"
    )
  endforeach()

  target_link_libraries(4JLibs.Windows64.Render PUBLIC 4JLibs.zlib)
endif()

function(minecraft_link_4jlibs target)
  if(MINECRAFT_4JLIBS_SOURCE_BUILD)
    target_link_libraries(${target} PRIVATE
      4JLibs.Windows64.Input
      4JLibs.Windows64.Storage
      4JLibs.Windows64.Render
    )
  else()
    target_link_libraries(${target} PRIVATE
      $<$<CONFIG:Debug,DebugFast>: # Debug 4J libraries
        "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_NAME}/4JLibs/libs/4J_Input_d.lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_NAME}/4JLibs/libs/4J_Storage_d.lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_NAME}/4JLibs/libs/4J_Render_PC_d.lib"
      >
      $<$<NOT:$<CONFIG:Debug,DebugFast>>: # Release 4J libraries
        "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_NAME}/4JLibs/libs/4J_Input.lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_NAME}/4JLibs/libs/4J_Storage.lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_NAME}/4JLibs/libs/4J_Render_PC.lib"
      >
    )
  endif()
endfunction()
