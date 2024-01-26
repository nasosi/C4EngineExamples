set( C4CurrentTargetName The31st )
project( ${C4CurrentTargetName} )

list ( APPEND C4ApplicationNames  ${C4CurrentTargetName} )
set( C4ApplicationNames ${C4ApplicationNames} PARENT_SCOPE )

file( GLOB SourceFiles ${C4GameCodeDir}/*.cpp )
file( GLOB HeaderFiles ${C4GameCodeDir}/*.h )

find_package( OpenGL REQUIRED )
set( C4SystemLibraries "${C4SystemLibraries} ${OPENGL_gl_LIBRARY}" )

set( WIN32Links C4Engine)

C4AddLibrary(
    ${C4CurrentTargetName}                  # Target Name
    "${SourceFiles}" "${HeaderFiles}"       # Sources
    "${C4EngineCodeDir} ${C4PluginCodeDir}" # Includes
    "${C4SystemLibraries}"                  # General links
    "${WIN32Links}"                         # Public Links specific to Windows and for the current target
    ""                                      # Relative Build path
)