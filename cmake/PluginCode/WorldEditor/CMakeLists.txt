set( C4CurrentTargetName WorldEditor )

project( ${C4CurrentTargetName} )

add_definitions( -DC4_EDITOR )

file( GLOB SourceFiles 
${C4PluginCodeDir}/C4CameraDirectors.cpp
${C4PluginCodeDir}/C4EditorDirectors.cpp
${C4PluginCodeDir}/C4EffectDirectors.cpp
${C4PluginCodeDir}/C4EmitterDirectors.cpp
${C4PluginCodeDir}/C4GeometryDirectors.cpp
${C4PluginCodeDir}/C4InstanceDirectors.cpp
${C4PluginCodeDir}/C4LightDirectors.cpp
${C4PluginCodeDir}/C4MarkerDirectors.cpp
${C4PluginCodeDir}/C4ModelDirectors.cpp
${C4PluginCodeDir}/C4PhysicsDirectors.cpp
${C4PluginCodeDir}/C4PortalDirectors.cpp
${C4PluginCodeDir}/C4SourceDirectors.cpp
${C4PluginCodeDir}/C4SpaceDirectors.cpp
${C4PluginCodeDir}/C4TriggerDirectors.cpp
${C4PluginCodeDir}/C4VolumeDirectors.cpp
${C4PluginCodeDir}/C4ZoneDirectors.cpp
${C4PluginCodeDir}/C4Landscaping.cpp
${C4PluginCodeDir}/C4TerrainBuilders.cpp
${C4PluginCodeDir}/C4TerrainTools.cpp
${C4PluginCodeDir}/C4Transformers.cpp
${C4PluginCodeDir}/C4WaterTools.cpp
${C4PluginCodeDir}/C4ModelViewer.cpp
${C4PluginCodeDir}/C4PanelEditor.cpp
${C4PluginCodeDir}/C4ScriptEditor.cpp
${C4PluginCodeDir}/C4ShaderEditor.cpp
${C4PluginCodeDir}/C4MaterialEditor.cpp
${C4PluginCodeDir}/C4EditorBase.cpp
${C4PluginCodeDir}/C4EditorBrush.cpp
${C4PluginCodeDir}/C4EditorCommands.cpp
${C4PluginCodeDir}/C4EditorConnectors.cpp
${C4PluginCodeDir}/C4EditorGizmo.cpp
${C4PluginCodeDir}/C4EditorSupplement.cpp
${C4PluginCodeDir}/C4EditorPages.cpp
${C4PluginCodeDir}/C4EditorPlugins.cpp
${C4PluginCodeDir}/C4EditorTools.cpp
${C4PluginCodeDir}/C4EditorOperations.cpp
${C4PluginCodeDir}/C4EditorViewports.cpp
${C4PluginCodeDir}/C4NodeInfo.cpp
${C4PluginCodeDir}/C4WorldEditor.cpp
${C4PluginCodeDir}/C4VertexEditor.cpp)

file( GLOB HeaderFiles
${C4PluginCodeDir}/C4CameraDirectors.h
${C4PluginCodeDir}/C4EditorDirectors.h
${C4PluginCodeDir}/C4EffectDirectors.h
${C4PluginCodeDir}/C4EmitterDirectors.h
${C4PluginCodeDir}/C4GeometryDirectors.h
${C4PluginCodeDir}/C4InstanceDirectors.h
${C4PluginCodeDir}/C4LightDirectors.h
${C4PluginCodeDir}/C4MarkerDirectors.h
${C4PluginCodeDir}/C4ModelDirectors.h
${C4PluginCodeDir}/C4PhysicsDirectors.h
${C4PluginCodeDir}/C4PortalDirectors.h
${C4PluginCodeDir}/C4SourceDirectors.h
${C4PluginCodeDir}/C4SpaceDirectors.h
${C4PluginCodeDir}/C4TriggerDirectors.h
${C4PluginCodeDir}/C4VolumeDirectors.h
${C4PluginCodeDir}/C4ZoneDirectors.h
${C4PluginCodeDir}/C4Landscaping.h
${C4PluginCodeDir}/C4TerrainBuilders.h
${C4PluginCodeDir}/C4TerrainTools.h
${C4PluginCodeDir}/C4WaterTools.h
${C4PluginCodeDir}/C4ModelViewer.h
${C4PluginCodeDir}/C4PanelEditor.h
${C4PluginCodeDir}/C4ScriptEditor.h
${C4PluginCodeDir}/C4ShaderEditor.h
${C4PluginCodeDir}/C4MaterialEditor.h
${C4PluginCodeDir}/C4EditorBase.h
${C4PluginCodeDir}/C4EditorBrush.h
${C4PluginCodeDir}/C4EditorCommands.h
${C4PluginCodeDir}/C4EditorConnectors.h
${C4PluginCodeDir}/C4EditorGizmo.h
${C4PluginCodeDir}/C4EditorSupplement.h
${C4PluginCodeDir}/C4EditorPages.h
${C4PluginCodeDir}/C4EditorPlugins.h
${C4PluginCodeDir}/C4EditorTools.h
${C4PluginCodeDir}/C4EditorOperations.h
${C4PluginCodeDir}/C4EditorViewports.h
${C4PluginCodeDir}/C4NodeInfo.h
${C4PluginCodeDir}/C4WorldEditor.h 
${C4PluginCodeDir}/C4VertexEditor.h
)

set( WIN32Links C4Engine )

C4AddLibrary(
    ${C4CurrentTargetName}                  # Target Name
    "${SourceFiles}" "${HeaderFiles}"           # Sources
    "${C4EngineCodeDir} ${C4PluginCodeDir} ${C4TerathonCodeDir}" # Includes
    "${C4SystemLibraries}"                     # General links
    "${WIN32Links}"                # Public Links specific to Windows and for
                                    # the current target
    Plugins/         # Install dir
)

if ( ${C4InstallPlugins} )
    install(TARGETS ${C4CurrentTargetName} DESTINATION bin/Plugins/Tools )
endif()