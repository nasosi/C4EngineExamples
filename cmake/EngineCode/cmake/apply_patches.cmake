# Change default infinite shadow map size 
set( C4Graphics_cppReplacementFile ${CMAKE_CURRENT_BINARY_DIR}/C4Graphics_rep.cpp )

if ( NOT C4InfiniteShadowResolution STREQUAL "0")
     if( EXISTS ${C4Graphics_cppReplacementFile})
        file(READ ${C4Graphics_cppReplacementFile} C4Graphicsrepdata)
    else()
        file(READ ${C4EngineCodeDir}/C4Graphics.cpp C4Graphicsrepdata)
    endif()

    string(REGEX MATCH "kInfiniteShadowResolution([ \t\r\n]*)=([ \t\r\n]*)[0-9]*" CurrentInfiniteShadowMapSize "${C4Graphicsrepdata}")
    string(REPLACE "kInfiniteShadowResolution" "" CurrentInfiniteShadowMapSize ${CurrentInfiniteShadowMapSize})
    string(REPLACE "=" "" CurrentInfiniteShadowMapSize ${CurrentInfiniteShadowMapSize})
    string(STRIP CurrentInfiniteShadowMapSize ${CurrentInfiniteShadowMapSize})
    string(REPLACE "\t" "" CurrentInfiniteShadowMapSize ${CurrentInfiniteShadowMapSize})
    string(REPLACE " " "" CurrentInfiniteShadowMapSize ${CurrentInfiniteShadowMapSize})

    if ( NOT ${C4InfiniteShadowResolution} STREQUAL ${CurrentInfiniteShadowMapSize})
        message( STATUS "Overriding kInfiniteShadowResolution")
        file(READ ${C4EngineCodeDir}/C4Graphics.cpp C4Graphicsdata)
        string( REGEX REPLACE "kInfiniteShadowResolution([ \t\r\n]*)=([ \t\r\n]*)[0-9]*" "kInfiniteShadowResolution\t\t\t= ${C4InfiniteShadowResolution}" C4Graphicsdata "${C4Graphicsdata}")
        file( WRITE ${C4Graphics_cppReplacementFile} "${C4Graphicsdata}" )
    endif()
endif()


# Modify C4Engine source so that we can use to import resources at compile time.
if ( ${C4EnableBuildTimeImport} )
    set( C4Engine_cppReplacementFile ${CMAKE_CURRENT_BINARY_DIR}/C4Engine_rep.cpp )

    # Move ExecuteText(commandLine) after PluginMgr::New();
    if( NOT EXISTS ${C4Engine_cppReplacementFile} ) 
        file(READ ${C4EngineCodeDir}/C4Engine.cpp C4Enginerepdata)

        string(REPLACE "ExecuteText(commandLine);" "" C4Enginerepdata "${C4Enginerepdata}")
        string( REPLACE "PluginMgr::New();" "PluginMgr::New();\n\n\tExecuteText(commandLine);" C4Enginerepdata "${C4Enginerepdata}")
        file( WRITE ${C4Engine_cppReplacementFile} "${C4Enginerepdata}" )
    endif()
endif()

# Finalize list of replacement files:
if( EXISTS ${C4Graphics_cppReplacementFile} AND NOT C4InfiniteShadowResolution STREQUAL "0")
    list( APPEND C4SourceFiles ${C4Graphics_cppReplacementFile})
else()
    list( APPEND C4SourceFiles ${C4EngineCodeDir}/C4Graphics.cpp)
endif()

if ( ${C4EnableBuildTimeImport} )
    list( APPEND C4SourceFiles ${C4Engine_cppReplacementFile})
else()
    list( APPEND C4SourceFiles ${C4EngineCodeDir}/C4Engine.cpp)
endif()