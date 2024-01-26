set( FOUNDAPP NO )
foreach( C4AppName ${C4ApplicationNames} )
    if( ${C4AppName} STREQUAL ${C4ActiveApp})
        set( FOUNDAPP YES )
    endif( )
endforeach( )

if( NOT ${FOUNDAPP} )
    message( FATAL_ERROR "Error : Active app `" ${C4ActiveApp} "` not found in the list of apps. Valid selections are:\n" "${C4ApplicationNames}" )
endif( )

message( STATUS "\n\n")
message( STATUS "================================================================================")
message( STATUS "Build setup.")
message( STATUS "================================================================================")

message( STATUS "Project name           : " ${CMAKE_PROJECT_NAME} )

message( STATUS "Binary destination dir : " ${C4BinaryDestDir} )

message( STATUS "Overrides of default C4 Engine options: " )

if ( NOT C4InfiniteShadowResolution STREQUAL "0" )
    message( STATUS "    > kInfiniteShadowResolution = " ${C4InfiniteShadowResolution})
endif()

message( STATUS "Applications to build  :" )

foreach( C4AppName ${C4ApplicationNames} )
    if( ${C4AppName} STREQUAL ${C4ActiveApp})
        message( STATUS "    > " ${C4AppName}  " (ACTIVE)")
    else( )
        message( STATUS "    > " ${C4AppName} )
    endif( )
endforeach( )