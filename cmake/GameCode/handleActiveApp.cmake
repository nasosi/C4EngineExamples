get_filename_component( CurrentAppName "The31st" NAME )

message( STATUS "Setting active app: " ${CurrentAppName} )

install(TARGETS ${CurrentAppName}  LIBRARY  DESTINATION bin )

if ( C4PackUponInstall )
    install( CODE "message( STATUS \"Packing The31st data...\")")
    install( CODE "execute_process( COMMAND C4Engine pack The31st\; quit WORKING_DIRECTORY ${C4ProgramRootDir})")
    install( FILES ${C4ProgramRootDir}/Data/The31st.pak DESTINATION bin/Data)
    install( CODE "file(REMOVE ${C4BinaryDestDir}/Data/The31st.pak )" )
else()
    if ( ${C4ForceCopyThe31stData} )
        install( DIRECTORY ${C4ProgramRootDir}/Data/The31st DESTINATION bin/Data)
    else()
        install( DIRECTORY ${C4DataRoot}/Data/The31st DESTINATION bin/Data)
    endif()
endif()
