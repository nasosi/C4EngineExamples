function ( C4AddLibrary
    TargetName
    SourceFiles HeaderFiles
    IncludeDirectories
    C4SystemLibraries
    WIN32Links
    RelativeOutputPath )

    add_library ( ${TargetName} SHARED ${SourceFiles} ${HeaderFiles} )

    separate_arguments( IncludeDirectories )
    target_include_directories( ${TargetName} PRIVATE ${IncludeDirectories} )

    set_target_properties( ${TargetName} PROPERTIES PREFIX "" )

    target_compile_definitions( ${TargetName} PRIVATE ${C4BuildTypeDefinition} TERATHON_IMPORT TERATHON_NO_SYSTEM )

    target_compile_features(${C4CurrentTargetName} PUBLIC cxx_std_17)

    set_target_properties( ${TargetName}
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${C4BinaryDestDir}/${RelativeOutputPath}"
        RUNTIME_OUTPUT_DIRECTORY "${C4BinaryDestDir}/${RelativeOutputPath}"
    )

    separate_arguments( C4SystemLibraries )

    if (WIN32)
        target_link_libraries( ${TargetName} PRIVATE ${C4SystemLibraries} PUBLIC ${WIN32Links} )
    else()
        message(Error "The C4 Engine currently only supports windows")
    endif(WIN32)

endfunction( C4AddLibrary )

function( C4CreateSymlink SymLink SymLinkTarget )

    file (TO_NATIVE_PATH ${SymLink} SymLink)
    file (TO_NATIVE_PATH ${SymLinkTarget} SymLinkTarget )
    execute_process( COMMAND "cmd.exe" "/C" "mklink" "/D" "/J" ${SymLink} ${SymLinkTarget})

endfunction( C4CreateSymlink )

function( C4IsNotReparsePoint dirname isreparse )
    file (TO_NATIVE_PATH ${dirname} dirname )
    execute_process( COMMAND "fsutil" reparsepoint query ${dirname} RESULT_VARIABLE res OUTPUT_VARIABLE out)
    set( "${isreparse}" false PARENT_SCOPE )
    if ( ${res} MATCHES 0)
        set( "${isreparse}" true PARENT_SCOPE )
    endif()
endfunction( C4IsNotReparsePoint)

function( C4CopyOrLink ForceCopy FromPath ToPath Level1Folder Level2Folder)

if (EXISTS ${FromPath}/${Level1Folder}/${Level2Folder} )
    C4IsNotReparsePoint(${ToPath}/${Level1Folder}/${Level2Folder} "isSymlink" )
    if ( ${ForceCopy}  )
        if ( ${isSymlink} )
            file( REMOVE_RECURSE ${ToPath}/${Level1Folder}/${Level2Folder} )
        endif()
        if ( NOT EXISTS ${ToPath}/${Level1Folder}/${Level2Folder} )
            message( STATUS "Copying ${Level2Folder} ${Level1Folder} over to ${ToPath}/${Level1Folder}" )
            file( COPY ${FromPath}/${Level1Folder}/${Level2Folder} DESTINATION ${ToPath}/${Level1Folder} )
        endif()
    else()
        if ( NOT ${isSymlink} AND EXISTS ${ToPath}/${Level1Folder}/${Level2Folder})
            file( REMOVE_RECURSE ${ToPath}/${Level1Folder}/${Level2Folder} )
            file( REMOVE ${ToPath}/${Level1Folder}/${Level2Folder} )
        endif()
        if ( NOT EXISTS ${ToPath}/${Level1Folder}/${Level2Folder} )
            C4CreateSymlink( 
                ${ToPath}/${Level1Folder}/${Level2Folder}
                ${FromPath}/${Level1Folder}/${Level2Folder} )
            endif()
    endif()
else( )
    message( FATAL_ERROR "C4Engine ${Level1Folder}/${Level2Folder} not found. Please set the C4DataRoot cmake variable to point to the top-level of C4 Engine data directory. Variable currently set to: " ${C4DataRoot}  )

endif( )
endfunction( C4CopyOrLink)


function( C4AddStringImportTarget C4BinaryDestDir ImportSubDir ImportFileName LatestTargetNameInOut)
    # The following  works if ExecuteText(commandLine); is placed after
    # PluginMgr::New() in C4Engine.cpp. We have provided a patch to do this.

    add_custom_command(
        OUTPUT "${C4BinaryDestDir}/Data/${ImportSubDir}/${ImportFileName}.str"
        DEPENDS "${C4BinaryDestDir}/Import/${ImportSubDir}/${ImportFileName}.txt" StringImporter
        COMMAND "C4Engine"
        ARGS istring ${ImportSubDir}/${ImportFileName}\; quit 
        WORKING_DIRECTORY ${C4BinaryDestDir}
        COMMENT "String import: istring ${ImportSubDir}/${ImportFileName}; quit;"
    )

    set(CurrentTargetName ${CurrentAppName}_Import_${ImportFileName})

    add_custom_target( ${CurrentAppName}_Import_${ImportFileName} ALL DEPENDS "${C4BinaryDestDir}/Data/${ImportSubDir}/${ImportFileName}.str" ${${LatestTargetNameInOut}})

    set(${LatestTargetNameInOut} ${CurrentTargetName} PARENT_SCOPE)

    message( STATUS "New C4 engine import:" ${${LatestTargetNameInOut}})

endfunction( C4AddStringImportTarget )

function( C4AddTextureImportTarget C4BinaryDestDir ImportSubDir ImportFileName LatestTargetNameInOut)
    # In first import the engine will fire up and ask to configure the texture import. From then, if the texture changes the same config will load.
    # TODO: Possibly provide a generic game that starts when calling import.
    # The following  works if ExecuteText(commandLine); is placed after
    # PluginMgr::New() in C4Engine.cpp. We have provided a patch to do this.

    add_custom_command(
        OUTPUT "${C4BinaryDestDir}/Data/${ImportSubDir}/${ImportFileName}.tex"
        DEPENDS "${C4BinaryDestDir}/Import/${ImportSubDir}/${ImportFileName}.tga" TextureTool
        COMMAND "C4Engine"
        ARGS itexture ${ImportSubDir}/${ImportFileName} # No quit here because we need to wait for user input
        WORKING_DIRECTORY ${C4BinaryDestDir}
        COMMENT "Texture import: itexture ${ImportSubDir}/${ImportFileName};"
     )

    set(CurrentTargetName ${CurrentAppName}_Import_${ImportFileName})

    add_custom_target( ${CurrentTargetName} ALL DEPENDS "${C4BinaryDestDir}/Data/${ImportSubDir}/${ImportFileName}.tex" ${${LatestTargetNameInOut}})

    set(${LatestTargetNameInOut} ${CurrentTargetName} PARENT_SCOPE)

    message("${C4BinaryDestDir}/Import/${ImportSubDir}/${ImportFileName}.tga")
    message( STATUS "${C4BinaryDestDir}/Data/${ImportSubDir}/${ImportFileName}.tex" )

endfunction( C4AddTextureImportTarget )

function( C4AddNormalImportTarget C4BinaryDestDir ImportSubDir ImportFileName LatestTargetNameInOut)
    # In first import the engine will fire up and ask to configure the normal texture import. From then, if the texture changes the same config will load.
    # TODO: Possibly provide a generic game that starts when calling import.
    # The following  works if ExecuteText(commandLine); is placed after
    # PluginMgr::New() in C4Engine.cpp. We have provided a patch to do this.

    add_custom_command(
        OUTPUT "${C4BinaryDestDir}/Data/${ImportSubDir}/${ImportFileName}.tex"
        DEPENDS "${C4BinaryDestDir}/Import/${ImportSubDir}/${ImportFileName}.tga" TextureTool
        COMMAND "C4Engine"
        ARGS inormal ${ImportSubDir}/${ImportFileName} # No quit here because we need to wait for user input
        WORKING_DIRECTORY ${C4BinaryDestDir}
        COMMENT "Texture import: inormal ${ImportSubDir}/${ImportFileName};"
     )

    set(CurrentTargetName ${CurrentAppName}_Normal_Import_${ImportFileName})

    add_custom_target( ${CurrentTargetName} ALL DEPENDS "${C4BinaryDestDir}/Data/${ImportSubDir}/${ImportFileName}.tex" ${${LatestTargetNameInOut}})

    set(${LatestTargetNameInOut} ${CurrentTargetName} PARENT_SCOPE)

    message("${C4BinaryDestDir}/Import/${ImportSubDir}/${ImportFileName}.tga")
    message( STATUS "${C4BinaryDestDir}/Data/${ImportSubDir}/${ImportFileName}.tex" )

endfunction( C4AddNormalImportTarget )
