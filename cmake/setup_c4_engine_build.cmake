if (EXISTS ${C4CodeRoot}/EngineCode/C4World.cpp )

    set( C4EngineCodeDir    ${C4CodeRoot}/EngineCode )
    set( C4GameCodeDir      ${C4CodeRoot}/GameCode )
    set( C4PluginCodeDir    ${C4CodeRoot}/PluginCode )
    set( C4TerathonCodeDir  ${C4CodeRoot}/TerathonCode)
    set( C4SlugCodeDir      ${C4CodeRoot}/SlugCode)

    include( ${C4CMakeDir}/cmake_macros/build_type_setup.cmake )
    include( ${C4CMakeDir}/cmake_macros/compiler_specific_setup.cmake )
    include( ${C4CMakeDir}/cmake_macros/platform_specific_setup.cmake )

    add_subdirectory( ${C4CMakeDir}/EngineCode )

    if ( ${C4BuildThe31st} )
        add_subdirectory( ${C4CMakeDir}/GameCode )
    endif()

    add_subdirectory( ${C4CMakeDir}/PluginCode )
else( )
    message( FATAL_ERROR "C4Engine Engine Code not found. Please set C4CodeRoot to point to the C4Engine code top-level directory" )
endif( )