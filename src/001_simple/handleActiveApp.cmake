get_filename_component( CurrentAppName "${CMAKE_CURRENT_LIST_DIR}" NAME )

message( STATUS "Setting active app: " ${CurrentAppName} )

file( REMOVE_RECURSE ${C4ProgramRootDir}/Data/Application)
file( REMOVE_RECURSE ${C4ProgramRootDir}/Import/Application)

file( MAKE_DIRECTORY  ${C4ProgramRootDir}/Data/Application )
file( MAKE_DIRECTORY  ${C4ProgramRootDir}/Import/Application )

# Link the entire Data folder
file( REMOVE ${C4ProgramRootDir}/Data/Application/${CurrentAppName} )
file( REMOVE_RECURSE ${C4ProgramRootDir}/Data/Application/${CurrentAppName} )
C4CreateSymlink( ${C4ProgramRootDir}/Data/Application/${CurrentAppName} ${CMAKE_CURRENT_LIST_DIR}/Data )

# Link the entire Import folder
file( REMOVE ${C4ProgramRootDir}/Import/Application/${CurrentAppName} )
file( REMOVE_RECURSE ${C4ProgramRootDir}/Import/Application/${CurrentAppName} )
C4CreateSymlink( ${C4ProgramRootDir}/Import/Application/${CurrentAppName} ${CMAKE_CURRENT_LIST_DIR}/Import )

if (${C4EnableBuildTimeImport})
    set( ImportDir               "Application/${CurrentAppName}/Game" )
    set( ImportStringFileName    "gameStrings" )
    C4AddStringImportTarget( ${C4BinaryDestDir} ${ImportDir} ${ImportStringFileName} C4LatestImportTarget)

    set( ImportTextureFileName "scribble")
    C4AddTextureImportTarget( ${C4BinaryDestDir} ${ImportDir} ${ImportTextureFileName} C4LatestImportTarget)
endif()

install(TARGETS ${CurrentAppName}  LIBRARY  DESTINATION bin )

if ( C4PackUponInstall )
    # If data gets too big, the pak file may need to be broken up
    install( CODE "execute_process( COMMAND C4Engine pack Application\; quit WORKING_DIRECTORY ${C4BinaryDestDir})")
    install( FILES ${C4BinaryDestDir}/Data/Application.pak DESTINATION bin/Data)
    install( CODE "file(REMOVE ${C4BinaryDestDir}/Data/Application.pak )" )
else()
    # Here we need to manually specify each of the Data folders
    install( DIRECTORY    
                ${CMAKE_CURRENT_LIST_DIR}/Data/Game 
                ${CMAKE_CURRENT_LIST_DIR}/Data/Panel 
                ${CMAKE_CURRENT_LIST_DIR}/Data/World
            DESTINATION 
                bin/Data/Application/${CurrentAppName} )
endif()
