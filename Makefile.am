AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I${srcdir}/include ${x11_CFLAGS} ${gl_CFLAGS} ${freetype2_CFLAGS} ${ftgl_CFLAGS} ${libpng_CFLAGS}

lib_LTLIBRARIES = libbwm801.la
libbwm801_la_SOURCES = src/core_gfx.cpp src/core_mapspace.cpp src/core_quad.cpp src/core_triplet.cpp src/Xwin_main.cpp src/core_main.cpp src/core_pair.cpp src/core_sockets_linux.cpp src/core_screenshot.cpp src/core_texture.cpp src/core_glext.cpp
libbwm801_la_LDFLAGS = -version-info 0:0:0


include_HEADERS = include/core_hexmap.hpp  include/core.hpp  include/core_isohexmap.hpp  include/core_mapbase.hpp

noinst_HEADERS = include/core_screenshot.h include/bwm801_glext.h

EXTRA_DIST = LICENSE README.md ${PACKAGE_NAME}-${API_VERSION}.pc

#licensedir= ${datarootdir}/licenses/${PACKAGE_NAME}
# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = ${PACKAGE_NAME}-${API_VERSION}.pc
# Install license and readme
#doc_DATA = README.md
#license_DATA = LICENSE 

#if INSTALLMOD
#if HAVEENVMOD
#modfile_DATA = ${PACKAGE_VERSION}
#CLEANFILES = ${PACKAGE_NAME}-${API_VERSION}.pc ${PACKAGE_VERSION}
#else
#modfile_DATA = ${PACKAGE_VERSION}.lua
#CLEANFILES = ${PACKAGE_NAME}-${API_VERSION}.pc ${PACKAGE_VERSION}.lua
#endif
#else
CLEANFILES = ${PACKAGE_NAME}-${API_VERSION}.pc
#endif
