AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I${srcdir}/include ${x11_CFLAGS} ${gl_CFLAGS} ${freetype2_CFLAGS} ${ftgl_CFLAGS} ${libpng_CFLAGS} -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror

bin_PROGRAMS = bwm801-launcher
bwm801_launcher_SOURCES = src/Xwin_bwm801_launcher.cpp
#bwm801_launcher_CFLAGS
bwm801_launcher_LDADD = -ldl

lib_LTLIBRARIES = libbwm801.la
libbwm801_la_SOURCES = src/bwm801_gfx.cpp src/bwm801_mapspace.cpp src/bwm801_quad.cpp src/bwm801_triplet.cpp src/Xwin_main.cpp src/bwm801_main.cpp src/bwm801_pair.cpp src/linux_bwm801_sockets.cpp src/bwm801_screenshot.cpp src/bwm801_texture.cpp src/bwm801_glext.cpp src/Xwin_bwm801_date_time.cpp src/Xwin_bwm801_crit_section.cpp src/bwm801_button_info.cpp  src/bwm801_calllist.cpp src/bwm801_pane.cpp src/bwm801_sockets.cpp src/bwm801_button_info_data.cpp src/bwm801_main_data.cpp src/bwm801_pane_data.cpp
libbwm801_la_LDFLAGS = -version-info 1:0:0


include_HEADERS = include/bwm801_button_info.h include/bwm801_calllist.h include/bwm801_comm.h include/bwm801_critsec.h include/bwm801_datetime.h include/bwm801_glext.h include/bwm801.h include/bwm801_hexmap.h include/bwm801_hexmap_inst.h include/bwm801_isohexmap.h include/bwm801_isohexmap_inst.h include/bwm801_main.h include/bwm801_mapbase.h include/bwm801_mapbase_inst.h include/bwm801_mapspace.h include/bwm801_pair.h include/bwm801_pane.h include/bwm801_quad.h include/bwm801_texture.h include/bwm801_triplet.h

noinst_HEADERS = include/bwm801_button_info_data.h include/bwm801_main_data.h include/bwm801_pane_data.h include/bwm801_screenshot.h include/bwm801_top.h

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
#CLEANFILES = ${PACKAGE_VERSION} ${PACKAGE_NAME}-${API_VERSION}.pc
#else
#modfile_DATA = ${PACKAGE_VERSION}.lua
#CLEANFILES = ${PACKAGE_VERSION}.lua ${PACKAGE_NAME}-${API_VERSION}.pc
#endif
#else
CLEANFILES = ${PACKAGE_NAME}-${API_VERSION}.pc
#endif


