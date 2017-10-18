AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I${srcdir}/include ${x11_CFLAGS} ${gl_CFLAGS} ${freetype2_CFLAGS} ${ftgl_CFLAGS}

lib_LIBRARIES = libbwm801.a
libbwm801_a_SOURCES = src/core_gfx.cpp src/core_mapspace.cpp src/core_quad.cpp src/core_triplet.cpp src/Xwin_main.cpp src/core_main.cpp src/core_pair.cpp src/core_sockets_linux.cpp

include_HEADERS = include/core_hexmap.hpp  include/core.hpp  include/core_isohexmap.hpp  include/core_mapbase.hpp

# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = bwm801-${API_VERSION}.pc


#dist_data_DATA = $(PACKAGE_NAME)
#everything below here creates a modulefile in the user's privatemodules directory (if it exists) or in the data directory
# the module will be created in a directory called ${PACKAGE_NAME} and have a filename of ${PACKAGE_VERSION}. This is the format used by modules for versioning.
if NON_DEFAULT_PREFIX
if ENVMODPINST

if PRIVMODINST
MODFILEINSTPATH = `echo ${HOME}"/privatemodules/"${PACKAGE_NAME}`
MODFILEINSTPATHV = `echo ${HOME}"/privatemodules/"${PACKAGE_NAME}"/"${PACKAGE_VERSION}`
MODFILEUSERMESSAGE = "A module was installed in "
MODFILEUSERMESSAGE += ${MODFILEINSTPATH}
MODFILEUSERMESSAGE += "."
else
MODFILEINSTPATH = `echo ${datadir}"/"${PACKAGE_NAME}`
MODFILEINSTPATHV = `echo ${datadir}"/"${PACKAGE_NAME}"/"${PACKAGE_VERSION}`
MODFILEUSERMESSAGE = "The path for module installaction couldn't be found. It was placed in "
MODFILEUSERMESSAGE += ${MODFILEINSTPATH}
MODFILEUSERMESSAGE += "."
endif

install-data-local:
	-$(MKDIR_P) ${MODFILEINSTPATH}
	@echo "#%Module1.0#####################################################################" > ${MODFILEINSTPATHV}
	@echo "##" >>  ${MODFILEINSTPATHV}
	@echo "## "${PACKAGE_NAME}" modulefile" >>  ${MODFILEINSTPATHV}
	@echo "##" >>  ${MODFILEINSTPATHV}
	@echo "## modulefiles/"${PACKAGE_NAME} >>  ${MODFILEINSTPATHV}
	@echo "##" >>  ${MODFILEINSTPATHV}
	@echo "proc ModulesHelp { } {" >>  ${MODFILEINSTPATHV}
	@echo "	global version" >>  ${MODFILEINSTPATHV}
	@echo "" >>  ${MODFILEINSTPATHV}
	@echo "	puts stderr \"\tAdds BWM801 engine to your PATH environment variable, c INCLUDE paths, and library load path\"" >>  ${MODFILEINSTPATHV}
	@echo "}" >>  ${MODFILEINSTPATHV}
	@echo "" >>  ${MODFILEINSTPATHV}
	@echo "module-whatis	\"adds BWM801 engine to your PATH environment variable, c INCLUDE paths, and library load path\"" >>  ${MODFILEINSTPATHV}
	@echo "" >>  ${MODFILEINSTPATHV}
	@echo "# for Tcl script use only" >>  ${MODFILEINSTPATHV}
	@echo "set	version		"${PACKAGE_VERSION} >>  ${MODFILEINSTPATHV}
	@echo "set	apppath		"${exec_prefix} >>  ${MODFILEINSTPATHV}
	@echo "" >>  ${MODFILEINSTPATHV}
	@echo 'append-path	PATH	$$apppath/bin' >>  ${MODFILEINSTPATHV}
	@echo 'append-path	C_INCLUDE_PATH	$$apppath/include' >>  ${MODFILEINSTPATHV}
	@echo 'append-path	CPLUS_INCLUDE_PATH $$apppath/include' >>  ${MODFILEINSTPATHV}
	@echo 'append-path	LD_LIBRARY_PATH $$apppath/lib' >>  ${MODFILEINSTPATHV}
	@echo 'append-path	LIBRARY_PATH $$apppath/lib' >>  ${MODFILEINSTPATHV}
	@echo ${MODFILEUSERMESSAGE}


endif
endif
