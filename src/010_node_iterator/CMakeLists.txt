# Setting the target name according to the directory name
get_filename_component( DIR_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
set( C4CurrentTargetName ${DIR_NAME} )

project( ${C4CurrentTargetName} )

list ( APPEND C4ApplicationNames  ${C4CurrentTargetName} )
set( C4ApplicationNames ${C4ApplicationNames} PARENT_SCOPE )

if ("${C4ActiveApp}" STREQUAL "")
    set( C4ActiveApp ${C4CurrentTargetName} )
    set( C4ActiveApp ${C4ActiveApp} PARENT_SCOPE )
endif( )

file( GLOB SourceFiles *.cpp )
file( GLOB HeaderFiles *.hpp )

add_library ( ${C4CurrentTargetName} MODULE ${SourceFiles} ${HeaderFiles} ) # MODULE because we don't care about the .lib. If C4 was structured differently then we might need a shared library

target_include_directories( ${C4CurrentTargetName} PRIVATE ${C4EngineCodeDir} ${C4PluginCodeDir})

separate_arguments( C4SystemLibraries )
target_link_libraries( ${C4CurrentTargetName}  ${C4SystemLibraries} )
if ( WIN32 )
    target_link_libraries( ${C4CurrentTargetName} LINK_PUBLIC  C4Engine )
endif( )
set_target_properties( ${C4CurrentTargetName} PROPERTIES PREFIX "" )

target_compile_features(${C4CurrentTargetName} PUBLIC cxx_std_17)

target_compile_definitions( ${C4CurrentTargetName} PRIVATE ${C4BuildTypeDefinition} TERATHON_IMPORT APPNAME=\"${C4CurrentTargetName}\")

set_target_properties( ${C4CurrentTargetName}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${C4BinaryDestDir}"
    LIBRARY_OUTPUT_DIRECTORY "${C4BinaryDestDir}"
    RUNTIME_OUTPUT_DIRECTORY "${C4BinaryDestDir}"
)

if(MSVC)
  target_compile_options(${C4CurrentTargetName} PRIVATE /W4 /WX)
else()
  target_compile_options(${C4CurrentTargetName} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

