cmake_minimum_required (VERSION 2.6)

project (superbible7plus)

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/" ${CMAKE_MODULE_PATH})
find_package(OpenGL)
find_package(OGLplus REQUIRED)

set (CMAKE_DEBUG_POSTFIX "_d")

if(WIN32)
    set(COMMON_LIBS sb7 optimized glfw3 debug glfw3_d ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})
elseif (UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    set(COMMON_LIBS sb7 ${GLFW_LIBRARIES} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL rt dl)
else()
    set(COMMON_LIBS sb7)
endif()

set(COMMON_LIBS ${COMMON_LIBS} ${EXTRA_LIBS})

add_library(sb7
            src/sb7/sb7.cpp
            src/sb7/sb7color.cpp
            src/sb7/sb7ktx.cpp
            src/sb7/sb7object.cpp
            src/sb7/sb7shader.cpp
            src/sb7/sb7textoverlay.cpp
            src/sb7/gl3w.c
)

    # Example framework copied from oglplus, since it's not normally installed.
    #  Opted against putting this in extern/, since that seems to contain entire
    #  snapshots of external libraries, while these are just a few internal files.
add_library(oglplus_example
            src/oglplus/example/glfw3_main.cpp
)

    # For simplicity, we only support glfw3_main.  However, the oglplus source tree
    #  also contains support for: glfw1, egl, glut, glx, qt4gl, qt5gl, sdl, and wxgl.
target_link_libraries(oglplus_example ${GLFW_LIBRARIES})

set(RUN_DIR ${PROJECT_BINARY_DIR}/bin)

    # Examples that have been ported to oglplus.
set(EXAMPLES_PLUS
  movingtri
  simpleclear
  singlepoint
  singletri
)

    # Examples still using sb7, only.
set(EXAMPLES
  alienrain
  basicfbo
  bindlesstex
  blendmatrix
  blinnphong
  bumpmapping
  clipdistance
  csflocking
  compressrgtc
  cubemapenv
  cubicbezier
  cullindirect
  deferredshading
  depthclamp
  dflandscape
  dispmap
  dof
  envmapsphere
  equirectangular
  fontdemo
  fragcolorfrompos
  fragmentlist
  grass
  gsculling
  gslayered
  gsquads
  gstessellate
  hdrbloom
  hdrexposure
  hdrtonemap
  hqfilter
  indexedcube
  indirectmaterial
  instancedattribs
  julia
  ktxview
  linesmooth
  mirrorclampedge
  msaanative
  multidrawindirect
  multiscissor
  multiviewport
  noperspective
  normalviewer
  objectexploder
  ompparticles
  packetbuffer
  perpixelgloss
  phonglighting
  polygonsmooth
  prefixsum
  pmbfractal
  pmbstreaming
  prefixsum2d
  programinfo
  raytracer
  rimlight
  sampleshading
# sb6mrender
  sdfdemo
  shadowmapping
  shapedpoints
  simpletexcoords
  simpletexture
  sparsetexture
  spinnycube
  springmass
  ssao
  starfield
  stereo
  subroutines
  tessellatedcube
  tessellatedgstri
  tessellatedtri
  tessmodes
  tesssubdivmodes
  toonshading
  tunnel
  wrapmodes
)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

foreach(EXAMPLE ${EXAMPLES} ${EXAMPLES_PLUS})
  add_executable(${EXAMPLE} WIN32 src/${EXAMPLE}/${EXAMPLE}.cpp)
  set_property(TARGET ${EXAMPLE} PROPERTY DEBUG_POSTFIX _d)
  target_link_libraries(${EXAMPLE} ${COMMON_LIBS})
  if(MSVC)
    configure_file(${PROJECT_SOURCE_DIR}/build/templates/vs2013.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${EXAMPLE}.vcxproj.user @ONLY)
  endif(MSVC)
endforeach(EXAMPLE)

foreach(EXAMPLE ${EXAMPLES_PLUS})
  target_link_libraries(${EXAMPLE} oglplus_example)
endforeach(EXAMPLE)

IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  add_definitions(-D_LINUX)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

    # Adding src enables including src/oglplus/example.hpp as just "oglplus/example.hpp".
include_directories( include src )
include_directories(${OGLPLUS_INCLUDE_DIRS})

ADD_CUSTOM_TARGET(debug ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE:STRING=Debug ${PROJECT_SOURCE_DIR})
ADD_CUSTOM_TARGET(release ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE:STRING=Release ${PROJECT_SOURCE_DIR})
