find_program( C4MSSignTool signtool )

if ( NOT C4MSSignTool )

    message( STATUS "C4EngineExamples: signtool.exe was not found or is not in the path. Code signing is disabled" )

else( )

    message( STATUS "C4EngineExamples: signtool.exe was found." )

    set( C4CodeSigningToolOptions "/fd certHash /v /t http://timestamp.digicert.com" CACHE STRING "signtool options" )

    set( C4CodeSigningCertificatePFX CACHE FILEPATH ".pfx Code Signing Certficate File" )

    if( "${C4CodeSigningCertificatePFX}" STREQUAL "" )
        
        message( STATUS "C4EngineExamples: C4CodeSigningCertificatePFX CMake variable is empty. Code signing is disabled. Provide the path to a .pfx file to enable code signing.")

    else()

        if ( NOT EXISTS ${C4CodeSigningCertificatePFX} )
            
            message( FATAL_ERROR "C4EngineExamples: the provided certificate and key file (${C4CodeSigningCertificatePFX}) does not exist. Either provide a valid .pfx file or set C4CodeSigningCertificatePFX to an empty string in the CMake configuration.")
        
        endif()

        set( C4CodeSigningTarget "${CMAKE_INSTALL_PREFIX}/bin/C4Engine.exe" )

        set( C4CodeSigningArguments "/f ${C4CodeSigningCertificatePFX} ${C4CodeSigningToolOptions}")

        set( C4SignToolCommand "signtool sign ${C4CodeSigningArguments} ${C4CodeSigningTarget}")

        message( STATUS "C4EngineExamples: Code Signing file exists. Code signing is enabled.")

        install( CODE 
            "
            message( STATUS \"C4EngineExamples: Code signing ${C4CodeSigningTarget}\")

            execute_process( 
                COMMAND 
                    cmd /c \"${C4SignToolCommand}\"
                RESULT_VARIABLE
                    C4SignRes
                OUTPUT_VARIABLE 
                    C4SignOutput
                COMMAND_ECHO 
                    STDOUT
            )

            message( STATUS \${C4SignOutput} )

            if (\${C4SignRes} EQUAL 0)

                 message( STATUS \"C4EngineExamples: Code signing was succesful.\")

            else()

               message( FATAL_ERROR \"C4EngineExamples: Error signing: ${C4CodeSigningTarget}\")

            endif()
            "
        )

        if(C4UseNSIS)

            cmake_path( GET CMAKE_COMMAND PARENT_PATH C4PathToCMake )

            set (C4NsisOriginalTemplateIn "${C4PathToCMake}/../share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules/Internal/CPack/NSIS.template.in")

            set( C4BuildTimeNsisStemplate "${CMAKE_BINARY_DIR}/cmake/Modules/NSIS.template.in" )

            if ( EXISTS ${C4NsisOriginalTemplateIn} )
    
                configure_file(${C4NsisOriginalTemplateIn} ${C4BuildTimeNsisStemplate} COPYONLY )

                message( STATUS "C4EngineExamples: The NSIS.template.in for your cmake version (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}) was found in the cmake installation.")

            else()
            
                configure_file("${CMAKE_CURRENT_LIST_DIR}/CustomModules/NSIS.template.in" ${C4BuildTimeNsisStemplate} COPYONLY )

                message( WARNING "C4EngineExamples: The NSIS.template.in for your cmake version (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}) was not found. Using NSIS.template.in coming with C4EngineExamples and is known to work with at least CMake-3.27.")
            
            endif()
            
            list( APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}/cmake/Modules" )

            file( READ "${CMAKE_CURRENT_LIST_DIR}/NSISCodeSignSection.txt" CONTENTS )
            file( APPEND ${C4BuildTimeNsisStemplate} "${CONTENTS}" )

            set( CPACK_NSIS_FINALIZE_CMD "signtool sign ${C4CodeSigningArguments} %1" )

        endif()

        
    endif()

endif()

unset( C4MSSignTool )
