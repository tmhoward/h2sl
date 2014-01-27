# Try to find LCM
# Once done this will define
#  LCM_FOUND        - Indication as to whether LCM was found
#  LCM_INCLUDE_DIRS - The LCM include directories
#  LCM_LIBRARIES    - The libraries needed to use LCM
#  LCM_DEFINITIONS  - Compiler flags needed to use LCM

find_package (PkgConfig)
pkg_check_modules (PC_LCM QUIET liblcm)
set (LCM_DEFINITIONS ${PC_LCM_CFLAGS_OTHER})

find_path(LCM_INCLUDE_DIR NAMES lcm/lcm.h
    HINTS @CMAKE_INSTALL_PREFIX@/include ${PC_LCM_INCLUDEDIR} ${PC_LCM_INCLUDE_DIRS} /usr/include /usr/local/include)

find_library (LCM_LIBRARY NAMES liblcm lcm
    HINTS @CMAKE_INSTALL_PREFIX@/lib ${PC_LCM_LIBDIR} ${PC_LCM_LIBRARY_DIRS} /usr/lib /usr/local/lib)

set (LCM_LIBRARIES ${LCM_LIBRARY})
set (LCM_INCLUDE_DIRS ${LCM_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args (LCM DEFAULT_MSG
    LCM_LIBRARY LCM_INCLUDE_DIR)

mark_as_advanced (LCM_INCLUDE_DIR LCM_LIBRARY)
