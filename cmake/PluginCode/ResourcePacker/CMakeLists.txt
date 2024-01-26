set( C4CurrentTargetName ResourcePacker )

project( ${C4CurrentTargetName} )

file( GLOB SourceFiles ${C4PluginCodeDir}/C4ResourcePacker.cpp )
file( GLOB HeaderFiles ${C4PluginCodeDir}/C4ResourcePacker.h )

set( WIN32Links C4Engine WorldEditor )

C4AddLibrary(
    ${C4CurrentTargetName}                  # Target Name
    "${SourceFiles}" "${HeaderFiles}"       # Sources
    "${C4EngineCodeDir} ${C4PluginCodeDir}" # Includes
    "${C4SystemLibraries}"                  # General links
    "${WIN32Links}"                         # Public Links specific to Windows and for
                                            # the current target
    Plugins/Tools                           # Install dir
)

if ( ${C4InstallPlugins} )
    install(TARGETS ${C4CurrentTargetName} LIBRARY DESTINATION bin/Plugins/Tools )
endif()
