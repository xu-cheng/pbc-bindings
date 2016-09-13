if (PBC_INCLUDE_DIR AND PBC_LIBRARIES)
  set(PBC_FIND_QUIETLY TRUE)
endif ()

find_path(PBC_INCLUDE_DIR NAMES pbc/pbc.h)
find_library(PBC_LIBRARIES NAMES pbc libpbc)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PBC DEFAULT_MSG PBC_INCLUDE_DIR PBC_LIBRARIES)

mark_as_advanced(PBC_INCLUDE_DIR PBC_LIBRARIES)
