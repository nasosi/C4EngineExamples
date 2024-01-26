set( C4CurrentTargetName StringImporter )

project( ${C4CurrentTargetName} )

file( GLOB SourceFiles ${C4PluginCodeDir}/C4StringImporter.cpp )
file( GLOB HeaderFiles ${C4PluginCodeDir}/C4StringImporter.h )

set( WIN32Links "" )

C4AddLibrary(
    ${C4CurrentTargetName}                  # Target Name
    "${SourceFiles}" "${HeaderFiles}"           # Sources
    "${C4EngineCodeDir} ${C4PluginCodeDir}" # Includes
    "${C4SystemLibraries} C4Engine"                     # General links
    "${WIN32Links}"                # Public Links specific to Windows and for
                                    # the current target
    Plugins/Tools         # Install dir
)

if ( ${C4InstallPlugins} )
    install(TARGETS ${C4CurrentTargetName} DESTINATION bin/Plugins/Tools )
endif()