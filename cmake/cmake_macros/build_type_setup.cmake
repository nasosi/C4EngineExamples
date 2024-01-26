if ( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE  Release )
endif()

if (CMAKE_BUILD_TYPE MATCHES "Debug"  )
    set( C4BuildTypeDefinition TERATHON_DEBUG)
endif( )
