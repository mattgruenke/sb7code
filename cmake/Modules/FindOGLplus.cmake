###############################################################################
# Find OGLplus
#
# This sets the following variables:
#  OGLPLUS_FOUND - True if OGLplus was found.
#  OGLPLUS_INCLUDE_DIRS - Directories containing OGLplus header files.
#  OGLPLUS_LIBRARIES - OGLplus library files.
#  OGLPLUS_DEFINITIONS - Compiler switches required for using OGLplus.
#
# TBD:
#  OGLPLUS_VERSION - Package version.
#

include(FindPackageHandleStandardArgs)

find_path(OGLPLUS_INCLUDE_DIR oglplus/all.hpp)
find_library(OGLPLUS_LIBRARY NAMES oglplus)

    # library is optional - don't fail if not found.
find_package_handle_standard_args(OGLplus REQUIRED_VARS OGLPLUS_INCLUDE_DIR)

mark_as_advanced(OGLPLUS_INCLUDE_DIR OGLPLUS_LIBRARY)
set(OGLPLUS_LIBRARIES    ${OGLPLUS_LIBRARY} )
set(OGLPLUS_INCLUDE_DIRS ${OGLPLUS_INCLUDE_DIR} )

