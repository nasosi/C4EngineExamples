include(CMakeDependentOption)

# User input items
set (C4ActiveApp "" CACHE PATH "C4Engine active applicaton.")

set( C4CodeRoot ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "C4Engine Engine code root path." )

set( C4DataRoot ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "C4Engine Engine data root path." )

set( C4ForceCopyEngineImport FALSE CACHE BOOL "Copy the data import directory instead of creating a symlink.")

set( C4ForceCopyToolsData FALSE CACHE BOOL "Copy the tools data directory instead of creating a symlink.")
set( C4ForceCopyToolsImport FALSE CACHE BOOL "Copy the tools import directory instead of creating a symlink.")

set( C4BuildThe31st FALSE CACHE BOOL "Build the game \"The 31st\"." )
cmake_dependent_option( C4ForceCopyThe31stData "Copy the Te31st data directory instead of creating a symlink." FALSE "C4BuildThe31st" FALSE )
cmake_dependent_option( C4ForceCopyThe31stImport "Copy the Te31st import directory instead of creating a symlink." FALSE "C4BuildThe31st" FALSE )
cmake_dependent_option( C4ForceCopyDemoData "Copy the Tutorial data directory instead of creating a symlink." FALSE "C4BuildThe31st" FALSE )

set( C4PullTutorialData FALSE CACHE BOOL "Pull tutorial data." )
cmake_dependent_option( C4ForceCopyTutorialData "Copy the Demo data directory instead of creating a symlink." FALSE "C4PullTutorialData" FALSE )

set( C4InfiniteShadowResolution "0" CACHE STRING "If other than 0, override the default shadow map size (2048)." )

set( C4EnableBuildTimeImport TRUE CACHE BOOL "Modify C4 Engine source code to enable importing resources during build time.")

set( C4InstallPlugins FALSE CACHE BOOL "Install C4 Plugins alongside applications and data.")

set( C4PackUponInstall TRUE CACHE BOOL "Pack the data upon cmake install using C4Engine pack files. Affects the contents of the installer.")

option( C4UseNSIS "Use NSIS generator to produce an installer. Otherwise use zip to bundle together." OFF)