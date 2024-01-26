set( C4CurrentTargetName TextureTool )

project( ${C4CurrentTargetName} )

add_definitions( -DC4_TEXTURE )

file( GLOB SourceFiles 
${C4PluginCodeDir}/C4TerrainPalette.cpp 
${C4PluginCodeDir}/C4TextureGenerator.cpp 
${C4PluginCodeDir}/C4TextureImporter.cpp 
${C4PluginCodeDir}/C4TextureTool.cpp 
${C4PluginCodeDir}/C4TextureSynthesizer.cpp
${C4PluginCodeDir}/C4TextureViewer.cpp )

file( GLOB HeaderFiles
${C4PluginCodeDir}/C4TerrainPalette.h
${C4PluginCodeDir}/C4TextureGenerator.h
${C4PluginCodeDir}/C4TextureImporter.h
${C4PluginCodeDir}/C4TextureSynthesizer.h
${C4PluginCodeDir}/C4TextureTool.h
${C4PluginCodeDir}/C4TextureViewer.h )

set( WIN32Links "" )

C4AddLibrary(
    ${C4CurrentTargetName}                  # Target Name
    "${SourceFiles}" "${HeaderFiles}"           # Sources
    "${C4EngineCodeDir} ${C4PluginCodeDir}" # Includes
    "${C4SystemLibraries} WorldEditor C4Engine"                     # General links
    "${WIN32Links}"                # Public Links specific to Windows and for
                                    # the current target
   Plugins/Tools         # Install dir
)

if ( ${C4InstallPlugins} )
    install(TARGETS ${C4CurrentTargetName} DESTINATION bin/Plugins/Tools )
endif()