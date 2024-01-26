set( C4CurrentTargetName SlugImporter )

project( ${C4CurrentTargetName} )

set( SourceFiles 
    ${C4PluginCodeDir}/C4CharNames.cpp
    ${C4PluginCodeDir}/C4SlugImporter.cpp
    ${C4SlugCodeDir}/SLFontBuilder.cpp
    ${C4SlugCodeDir}/SLOpenVex.cpp
    ${C4SlugCodeDir}/SLOtfReader.cpp
    ${C4SlugCodeDir}/SLSvgReader.cpp
)

file( GLOB HeaderFiles ${C4SlugCodeDir}/*.h )

C4AddLibrary(
    ${C4CurrentTargetName}                  # Target Name
    "${SourceFiles}" "${HeaderFiles}"       # Sources
    "${C4EngineCodeDir} ${C4PluginCodeDir}" # Includes
    "${C4SystemLibraries}"                  # General links
    "C4Engine"                              # Public Links specific to Windows and for
                                            # the current target
    Plugins/Tools                           # Install dir
)

if ( ${C4InstallPlugins} )
    install(TARGETS ${C4CurrentTargetName} LIBRARY DESTINATION bin/Plugins/Tools )
endif()