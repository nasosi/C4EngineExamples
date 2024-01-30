set( C4CurrentTargetName C4Engine )

project( ${C4CurrentTargetName} )

set( C4SourceFiles 
    # Base
    ${C4EngineCodeDir}/C4+Windows.rc
    ${C4EngineCodeDir}/C4+Wintab.cpp 
    ${C4EngineCodeDir}/C4Memory.cpp
    # Base > TerathonCode
    ${C4TerathonCodeDir}/TSTools.cpp
    #Controllers
    ${C4EngineCodeDir}/C4Controller.cpp
    ${C4EngineCodeDir}/C4Expressions.cpp
    ${C4EngineCodeDir}/C4Methods.cpp
    ${C4EngineCodeDir}/C4Scripts.cpp
    ${C4EngineCodeDir}/C4Values.cpp
    # Effects
    ${C4EngineCodeDir}/C4Effects.cpp
    ${C4EngineCodeDir}/C4Emitters.cpp
    ${C4EngineCodeDir}/C4Fur.cpp
    ${C4EngineCodeDir}/C4Halo.cpp
    ${C4EngineCodeDir}/C4Markings.cpp
    ${C4EngineCodeDir}/C4Particles.cpp
    ${C4EngineCodeDir}/C4Rules.cpp
    ${C4EngineCodeDir}/C4Shafts.cpp
    # Geometry
    ${C4EngineCodeDir}/C4Geometries.cpp
    ${C4EngineCodeDir}/C4GeometryObjects.cpp
    ${C4EngineCodeDir}/C4Mesh.cpp
    ${C4EngineCodeDir}/C4Primitives.cpp
    ${C4EngineCodeDir}/C4Terrain.cpp
    ${C4EngineCodeDir}/C4Topology.cpp
    # Graphics
    ${C4EngineCodeDir}/C4Attributes.cpp
    ${C4EngineCodeDir}/C4CameraObjects.cpp
    ${C4EngineCodeDir}/C4ComputeShaders.cpp
    ${C4EngineCodeDir}/C4FragmentShaders.cpp
    ${C4EngineCodeDir}/C4GeometryShaders.cpp
    ${C4EngineCodeDir}/C4Horizon.cpp
    ${C4EngineCodeDir}/C4LightObjects.cpp
    ${C4EngineCodeDir}/C4MaterialObjects.cpp
    ${C4EngineCodeDir}/C4OpenGL.cpp
    ${C4EngineCodeDir}/C4Processes.cpp
    ${C4EngineCodeDir}/C4Programs.cpp
    ${C4EngineCodeDir}/C4Quads.cpp
    ${C4EngineCodeDir}/C4Render.cpp
    ${C4EngineCodeDir}/C4Renderable.cpp
    ${C4EngineCodeDir}/C4Shaders.cpp
    ${C4EngineCodeDir}/C4SpaceObjects.cpp
    ${C4EngineCodeDir}/C4TExtures.cpp
    ${C4EngineCodeDir}/C4VertexShaders.cpp
    ${C4EngineCodeDir}/C4Voxels.cpp
    # Interface
    ${C4EngineCodeDir}/C4ColorPicker.cpp
    ${C4EngineCodeDir}/C4Configurable.cpp
    ${C4EngineCodeDir}/C4Configuration.cpp
    ${C4EngineCodeDir}/C4Dialog.cpp
    ${C4EngineCodeDir}/C4FilePicker.cpp
    ${C4EngineCodeDir}/C4Interface.cpp
    ${C4EngineCodeDir}/C4Logo.cpp
    ${C4EngineCodeDir}/C4Menus.cpp
    ${C4EngineCodeDir}/C4Movies.cpp
    ${C4EngineCodeDir}/C4Paint.cpp
    ${C4EngineCodeDir}/C4Panels.cpp
    ${C4EngineCodeDir}/C4Slug.cpp
    ${C4EngineCodeDir}/C4Sprockets.cpp
    ${C4EngineCodeDir}/C4ToolWindows.cpp
    ${C4EngineCodeDir}/C4Viewports.cpp
    ${C4EngineCodeDir}/C4Widgets.cpp
    # Interface > Slug
    ${C4SlugCodeDir}/SLSlug.cpp
    ${C4SlugCodeDir}/SLUnicode.cpp
    # Math
    ${C4EngineCodeDir}/C4Computation.cpp
    ${C4EngineCodeDir}/C4Constants.cpp
    ${C4EngineCodeDir}/C4Hull.cpp
    ${C4EngineCodeDir}/C4Polyhedron.cpp
    ${C4EngineCodeDir}/C4Random.cpp
    ${C4EngineCodeDir}/C4Sphere.cpp
    # Math > Terathon
    ${C4TerathonCodeDir}/TSAlgebra.cpp
    ${C4TerathonCodeDir}/TSBezier.cpp
    ${C4TerathonCodeDir}/TSBivector3D.cpp
    ${C4TerathonCodeDir}/TSBox.cpp
    ${C4TerathonCodeDir}/TSCircle3D.cpp
    ${C4TerathonCodeDir}/TSColor.cpp
    ${C4TerathonCodeDir}/TSDipole3D.cpp
    ${C4TerathonCodeDir}/TSFlector3D.cpp
    ${C4TerathonCodeDir}/TSHalf.cpp
    ${C4TerathonCodeDir}/TSLine3D.cpp
    ${C4TerathonCodeDir}/TSMath.cpp
    ${C4TerathonCodeDir}/TSMatrix2D.cpp
    ${C4TerathonCodeDir}/TSMatrix3D.cpp
    ${C4TerathonCodeDir}/TSMatrix4D.cpp
    ${C4TerathonCodeDir}/TSMotor3D.cpp
    ${C4TerathonCodeDir}/TSPlane3D.cpp
    ${C4TerathonCodeDir}/TSQuaternion.cpp
    ${C4TerathonCodeDir}/TSRound3D.cpp
    ${C4TerathonCodeDir}/TSSphere3D.cpp
    ${C4TerathonCodeDir}/TSVector2D.cpp
    ${C4TerathonCodeDir}/TSVector3D.cpp
    ${C4TerathonCodeDir}/TSVector4D.cpp
    # Objects
    ${C4EngineCodeDir}/C4Objects.cpp
    ${C4EngineCodeDir}/C4Volumes.cpp
    # Physics
    ${C4EngineCodeDir}/C4Blockers.cpp
    ${C4EngineCodeDir}/C4Body.cpp
    ${C4EngineCodeDir}/C4Character.cpp
    ${C4EngineCodeDir}/C4Contacts.cpp
    ${C4EngineCodeDir}/C4Deformable.cpp
    ${C4EngineCodeDir}/C4Fields.cpp
    ${C4EngineCodeDir}/C4Forces.cpp
    ${C4EngineCodeDir}/C4Inertia.cpp
    ${C4EngineCodeDir}/C4Joints.cpp
    ${C4EngineCodeDir}/C4Kinematic.cpp
    ${C4EngineCodeDir}/C4Physics.cpp
    ${C4EngineCodeDir}/C4Shapes.cpp
    ${C4EngineCodeDir}/C4Water.cpp
    # Plugins
    ${C4EngineCodeDir}/C4Plugins.cpp
    ${C4EngineCodeDir}/C4Sequence.cpp
    ${C4EngineCodeDir}/C4Targa.cpp
    # System
    ${C4EngineCodeDir}/C4Application.cpp
    ${C4EngineCodeDir}/C4Commands.cpp
    ${C4EngineCodeDir}/C4ConfigData.cpp
    ${C4EngineCodeDir}/C4Files.cpp
    ${C4EngineCodeDir}/C4Input.cpp
    ${C4EngineCodeDir}/C4Main.cpp
    ${C4EngineCodeDir}/C4Messages.cpp
    ${C4EngineCodeDir}/C4Network.cpp
    ${C4EngineCodeDir}/C4Packing.cpp
    ${C4EngineCodeDir}/C4Resources.cpp
    ${C4EngineCodeDir}/C4Sound.cpp
    ${C4EngineCodeDir}/C4StringTable.cpp
    ${C4EngineCodeDir}/C4Threads.cpp
    ${C4EngineCodeDir}/C4Time.cpp
    ${C4EngineCodeDir}/C4Variables.cpp
    # System > Terathon
    ${C4TerathonCodeDir}/TSCompression.cpp
    ${C4TerathonCodeDir}/TSData.cpp
    ${C4TerathonCodeDir}/TSOpenDDL.cpp
    # Utilities
    # Utilities > Terathon
    ${C4TerathonCodeDir}/TSBuffer.cpp
    ${C4TerathonCodeDir}/TSGraph.cpp
    ${C4TerathonCodeDir}/TSHash.cpp
    ${C4TerathonCodeDir}/TSList.cpp
    ${C4TerathonCodeDir}/TSMap.cpp
    ${C4TerathonCodeDir}/TSString.cpp
    ${C4TerathonCodeDir}/TSText.cpp
    ${C4TerathonCodeDir}/TSTree.cpp
    # World
    ${C4EngineCodeDir}/C4Animation.cpp
    ${C4EngineCodeDir}/C4Cameras.cpp
    ${C4EngineCodeDir}/C4Cell.cpp
    ${C4EngineCodeDir}/C4Connector.cpp
    ${C4EngineCodeDir}/C4Director.cpp
    ${C4EngineCodeDir}/C4Impostors.cpp
    ${C4EngineCodeDir}/C4Instances.cpp
    ${C4EngineCodeDir}/C4Lights.cpp
    ${C4EngineCodeDir}/C4Markers.cpp
    ${C4EngineCodeDir}/C4Models.cpp
    ${C4EngineCodeDir}/C4Modifiers.cpp
    ${C4EngineCodeDir}/C4Node.cpp
    ${C4EngineCodeDir}/C4Paths.cpp
    ${C4EngineCodeDir}/C4Portals.cpp
    ${C4EngineCodeDir}/C4Properties.cpp
    ${C4EngineCodeDir}/C4Regions.cpp
    ${C4EngineCodeDir}/C4Shakers.cpp
    ${C4EngineCodeDir}/C4Skybox.cpp
    ${C4EngineCodeDir}/C4Sources.cpp
    ${C4EngineCodeDir}/C4Spaces.cpp
    ${C4EngineCodeDir}/C4Triggers.cpp
    ${C4EngineCodeDir}/C4World.cpp
    ${C4EngineCodeDir}/C4Zones.cpp
)

set( SlugHeaderFiles 
    ${C4SlugCodeDir}/SLFragmentShader.h
    ${C4SlugCodeDir}/SLVertexShader.h
    ${C4SlugCodeDir}/SLUnicode.h
)
file( GLOB C4HeaderFiles ${C4EngineCodeDir}/*.h )
file( GLOB TerathonHeaderFiles ${C4TerathonCodeDir}/*.h)

include( ${C4CMakeDir}/cmake_macros/compiler_specific_setup.cmake )
include( ${C4CMakeDir}/cmake_macros/platform_specific_setup.cmake )

find_package( OpenGL REQUIRED )
set( C4SystemLibraries "${C4SystemLibraries} ${OPENGL_gl_LIBRARY}" )

include( cmake/apply_patches.cmake )

# Finalize compilation target
add_executable ( ${C4CurrentTargetName} WIN32 ${C4SourceFiles} ${C4HeaderFiles} ${TerathonHeaderFiles})

set_target_properties(${C4CurrentTargetName} PROPERTIES LINK_FLAGS
    "/DEF:\"${C4Modules}\" /RELEASE /OPT:REF /INCREMENTAL:NO /OPT:ICF /ERRORREPORT:PROMPT /NOLOGO /TLBID:1")

set_target_properties(${C4CurrentTargetName} PROPERTIES ENABLE_EXPORTS ON)

set_target_properties( ${C4CurrentTargetName}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${C4BinaryDestDir}/${RelativeOutputPath}"
    RUNTIME_OUTPUT_DIRECTORY "${C4BinaryDestDir}/${RelativeOutputPath}"
)

separate_arguments( C4SystemLibraries )
target_link_libraries( ${C4CurrentTargetName} ${C4SystemLibraries})
target_compile_features(${C4CurrentTargetName} PUBLIC cxx_std_17)
target_compile_definitions( ${C4CurrentTargetName} PRIVATE ${C4BuildTypeDefinition} TERATHON_EXPORT C4_ENGINE_MODULE TERATHON_NO_SYSTEM)
target_compile_definitions( ${C4CurrentTargetName} PUBLIC ${C4PlatformDefinition})
target_include_directories( ${C4CurrentTargetName} PUBLIC ${C4EngineCodeDir} ${C4TerathonCodeDir} ${C4SlugCodeDir})


install(TARGETS ${C4CurrentTargetName} RUNTIME DESTINATION bin)
