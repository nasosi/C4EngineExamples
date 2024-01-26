set( C4CurrentTargetName MovieTool )

project( ${C4CurrentTargetName} )

file( GLOB SourceFiles 
${C4PluginCodeDir}/C4MovieImporter.cpp 
${C4PluginCodeDir}/C4MoviePlayer.cpp 
${C4PluginCodeDir}/C4MovieTool.cpp)

file( GLOB HeaderFiles
${C4PluginCodeDir}/C4MovieImporter.h
${C4PluginCodeDir}/C4MoviePlayer.h
${C4PluginCodeDir}/C4MovieTool.h )

set( WIN32Links C4Engine SoundTool)

C4AddLibrary(
    ${C4CurrentTargetName}                  # Target Name
    "${SourceFiles}" "${HeaderFiles}"           # Sources
    "${C4EngineCodeDir} ${C4PluginCodeDir}" # Includes
    "${C4SystemLibraries}"                     # General links
    "${WIN32Links}"                # Public Links specific to Windows and for
                                    # the current target
    Plugins/Tools/Movies  # Install dir
    )

if ( ${C4InstallPlugins} )
    install(TARGETS ${C4CurrentTargetName} DESTINATION bin/${AppDirectory}/Plugins/Tools/Movies )
endif()
