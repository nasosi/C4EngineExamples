include( cmake/setup_code_signing.cmake )

if(C4UseNSIS)

  set(CPACK_GENERATOR "NSIS")

else(C4UseNSIS)

  set(CPACK_GENERATOR "ZIP")

endif(C4UseNSIS)

set(CPACK_PACKAGE_VENDOR "My Games" )

set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})

set(CPACK_PACKAGE_EXECUTABLES "C4Engine" ${C4ActiveApp})
set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\C4Engine.exe") # Alternatively: "MyAppIcon.ico""

set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY ${CMAKE_PROJECT_NAME})

set(CPACK_IGNORE_FILES "\.psd$;/CVS/;/\.svn/;/\.git/;\.swp$;/CMakeLists.txt.user;\.#;/#;\.tar.gz$;/CMakeFiles/;CMakeCache.txt;\.qm$;/build/;\.diff$;.DS_Store'")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_SOURCE_IGNORE_FILES ${CPACK_IGNORE_FILES})

#set(CPACK_NSIS_HELP_LINK ${HELP_URL})
#set(CPACK_NSIS_URL_INFO_ABOUT ${ABOUT_URL})
#set(CPACK_NSIS_CONTACT ${DEV_EMAIL})

include(CPack)

message( STATUS BBB ${CMAKE_BINARY_DIR} )
ADD_CUSTOM_TARGET( 
        Install 
    COMMAND 
        ${CMAKE_COMMAND} --install .
    WORKING_DIRECTORY 
        ${CMAKE_BINARY_DIR}
    VERBATIM
    COMMENT 
        "Running cmake --install Please wait..." DEPENDS C4Engine ${C4ActiveApp} )

ADD_CUSTOM_TARGET( Bundle COMMAND "${CMAKE_CPACK_COMMAND}" "-V" COMMENT "Running CPack. Please wait..." DEPENDS C4Engine ${C4ActiveApp} Install )