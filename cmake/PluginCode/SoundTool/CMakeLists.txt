set( C4CurrentTargetName SoundTool )

project( ${C4CurrentTargetName} )

add_definitions( -DC4_SOUND )

file( GLOB SourceFiles 
${C4PluginCodeDir}/C4SoundImporter.cpp 
${C4PluginCodeDir}/C4SoundPlayer.cpp 
${C4PluginCodeDir}/C4SoundTool.cpp )

file( GLOB HeaderFiles
${C4PluginCodeDir}/C4SoundImporter.h
${C4PluginCodeDir}/C4SoundPlayer.h
${C4PluginCodeDir}/C4SoundTool.h )

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
    install(TARGETS ${C4CurrentTargetName} DESTINATION bin/Plugins/Tools )
endif()