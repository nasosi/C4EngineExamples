cmake_minimum_required ( VERSION 3.17 )

project( C4EngineExamples VERSION 7.1.1 )

include( cmake/cmake_macros/c4functions.cmake )

include( cmake/setup_user_selectable_build_options.cmake )

include( cmake/init_default_variables.cmake )

include( cmake/copy_data_to_build_directory.cmake )

include( cmake/setup_c4_engine_build.cmake )

# Setup our applications
add_subdirectory( src )

# Set the game name in game.cfg, so that C4Engine knows which dll to load:
configure_file( ${C4CMakeDir}/game.cfg.in ${C4ProgramRootDir}/Data/Engine/game.cfg @ONLY )

include( cmake/setup_installer.cmake )

include( cmake/compute_and_display_build_setup.cmake )
