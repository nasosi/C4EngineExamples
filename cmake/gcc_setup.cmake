add_definitions( -DGLX_NV_copy_buffer=0 )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
-Wall -Wno-unused-function -Wno-switch -Wno-multichar -Wno-char-subscripts \
-Wno-enum-compare -Wno-uninitialized -Wno-non-virtual-dtor \
-Wno-delete-non-virtual-dtor -Wno-sign-compare -Wno-invalid-offsetof \
-Wno-deprecated-declarations -Wno-unused-result -Wno-array-bounds"
)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
-m64 -msse -msse2 -fwrapv -fno-strict-aliasing -fno-exceptions -fno-rtti" )

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} \
-fno-exceptions -fno-rtti" )

set( CMAKE_THREAD_PREFER_PTHREAD ON)
set( THREADS_PREFER_PTHREAD_FLAG ON)
find_package( Threads REQUIRED)
set( C4SystemLibraries "${C4SystemLibraries} Threads::Threads" )

set( C4SystemLibraries "${C4SystemLibraries} ${CMAKE_DL_LIBS}" )
