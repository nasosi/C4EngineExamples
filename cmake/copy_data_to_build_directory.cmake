set( ForceCopy true)
set( ForceLink false)
file( MAKE_DIRECTORY ${C4ProgramRootDir}/Import/ )

C4CopyOrLink( ${ForceCopy} ${C4DataRoot} ${C4ProgramRootDir} Data Engine )
if ( C4PackUponInstall )
    # If data gets too big, the pak file may need to be broken up
    install( CODE "message( STATUS \"Packing Engine data...\")")
    install( CODE "execute_process( COMMAND C4Engine pack Engine\; quit WORKING_DIRECTORY ${C4BinaryDestDir})")
    install( FILES ${C4BinaryDestDir}/Data/Engine.pak DESTINATION bin/Data)
    install( CODE "file(REMOVE ${C4BinaryDestDir}/Data/Engine.pak )" )
else()
    install( DIRECTORY ${C4ProgramRootDir}/Data/Engine DESTINATION bin/Data)
endif()

#C4CopyOrLink( ${C4ForceCopyEngineImport} ${C4DataRoot} ${C4ProgramRootDir} Import Engine )

C4CopyOrLink( ${C4ForceCopyToolsData} ${C4DataRoot} ${C4ProgramRootDir} Data Tools )

if ( ${C4InstallPlugins})
    if ( C4PackUponInstall )
        # If data gets too big, the pak file may need to be broken up
        install( CODE "message( STATUS \"Packing Tools data...\")")
        install( CODE "execute_process( COMMAND C4Engine pack Tools\; quit WORKING_DIRECTORY ${C4BinaryDestDir})")
        install( FILES ${C4BinaryDestDir}/Data/Tools.pak DESTINATION bin/Data)
        install( CODE "file(REMOVE ${C4BinaryDestDir}/Data/Tools.pak )" )
    else()
        if ( ${C4ForceCopyToolsData} )
            install( DIRECTORY ${C4ProgramRootDir}/Data/Tools DESTINATION bin/Data)
        else()
            install( DIRECTORY ${C4DataRoot}/Data/Tools DESTINATION bin/Data)
        endif()
    endif()
endif()

#C4CopyOrLink( ${C4ForceCopyToolsImport} ${C4DataRoot} ${C4ProgramRootDir} Import Tools )

if ( ${C4BuildThe31st} )
    C4CopyOrLink( ${C4ForceCopyThe31stData} ${C4DataRoot} ${C4ProgramRootDir} Data The31st )
    C4CopyOrLink( ${C4ForceCopyDemoData} ${C4DataRoot} ${C4ProgramRootDir} Data Demo )
endif()

if ( ${C4PullTutorialData} )
    C4CopyOrLink( ${C4ForceCopyTutorialData} ${C4DataRoot} ${C4ProgramRootDir} Data Tutorial )
endif()