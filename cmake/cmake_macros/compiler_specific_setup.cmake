if ( "${CMAKE_CXX_COMPILER_ID}"     STREQUAL "Clang" )

    message( WARNING "Clang has not been tested." )

    include( cmake/gcc_setup.cmake )
  
elseif ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )

    message( WARNING "The GCC compiler has not been tested." )

    include( cmake/gcc_setup.cmake )

elseif ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel" )

    message( WARNING "The Intel compiler has not been tested." )

    include( cmake/gcc_setup.cmake )
  
elseif ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )

   include( cmake/msvc_setup.cmake )
    
endif()
