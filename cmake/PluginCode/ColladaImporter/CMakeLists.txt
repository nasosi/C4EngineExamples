set( C4CurrentTargetName ColladaImporter )

project( ${C4CurrentTargetName} )

file( GLOB SourceFiles ${C4PluginCodeDir}/C4Collada.cpp )
file( GLOB HeaderFiles ${C4PluginCodeDir}/C4Collada.h )

set( WIN32Links C4Engine WorldEditor TextureTool )

C4AddLibrary(
    ${C4CurrentTargetName}                  # Target Name
    "${SourceFiles}" "${HeaderFiles}"       # Sources
    "${C4EngineCodeDir} ${C4PluginCodeDir}" # Includes
    "${C4SystemLibraries}"                  # General links
    "${WIN32Links}"                         # Public Links specific to Windows and for
                                            # the current target
    Plugins/Tools/Importers                 # Install dir
)

if ( ${C4InstallPlugins} )
    install(TARGETS ${C4CurrentTargetName} LIBRARY DESTINATION bin/Plugins/Tools/Importers )
endif()