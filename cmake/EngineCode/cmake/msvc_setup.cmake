set(C4Modules ${C4EngineCodeDir}/C4Module.def)

list( APPEND SourceFiles ${C4Modules} )

set_source_files_properties(${C4Modules} PROPERTIES HEADER_FILE_ONLY TRUE)