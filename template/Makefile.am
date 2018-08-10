AUTOMAKE_OPTIONS = foreign subdir-objects

AM_CPPFLAGS = -I${srcdir}/include ${bwm801_CFLAGS} -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror

lib_LTLIBRARIES = bwm801-user-template.la

bwm801_user_template_la_SOURCES = src/bwm801-user-template.cpp src/bwm801-user-template-gfx.cpp src/bwm801-user-template-UI.cpp
bwm801_user_template_la_LDFLAGS = ${bwm801_LIBS} -avoid-version -module -shared  -export-dynamic

extradatadir = $(prefix)/lib
extradata_DATA = spectrum.png
