AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I${srcdir}/include ${x11_CFLAGS} ${gl_CFLAGS} ${freetype2_CFLAGS} ${ftgl_CFLAGS} ${libpng_CFLAGS}

lib_LTLIBRARIES = libbwm801.la
libbwm801_la_SOURCES = src/core_gfx.cpp src/core_mapspace.cpp src/core_quad.cpp src/core_triplet.cpp src/Xwin_main.cpp src/core_main.cpp src/core_pair.cpp src/core_sockets_linux.cpp src/core_screenshot.cpp src/core_texture.cpp src/core_glext.cpp src/Xwin_bwm801_date_time.cpp src/Xwin_bwm801_crit_section.cpp src/bwm801_button_info.cpp  src/bwm801_calllist.cpp src/bwm801_pane.cpp src/bwm801_sockets.cpp
libbwm801_la_LDFLAGS = -version-info 1:0:0


include_HEADERS = include/bwm801_button_info.h include/bwm801_calllist.h include/bwm801_comm.h include/bwm801_critsec.h include/bwm801_datetime.h include/bwm801_glext.h include/bwm801.h include/bwm801_hexmap.h include/bwm801_hexmap_inst.h include/bwm801_isohexmap.h include/bwm801_isohexmap_inst.h include/bwm801_main.h include/bwm801_mapbase.h include/bwm801_mapbase_inst.h include/bwm801_mapspace.h include/bwm801_pair.h include/bwm801_pane.h include/bwm801_quad.h include/bwm801_texture.h include/bwm801_triplet.h

# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = ${PACKAGE_NAME}-${API_VERSION}.pc

if INSTALLMOD
if HAVEENVMOD
modfile_DATA = ${PACKAGE_VERSION}
else
modfile_DATA = ${PACKAGE_VERSION}.lua
endif
endif
