AUTOMAKE_OPTIONS = foreign subdir-objects

AM_CPPFLAGS = -I${srcdir}/include ${bwm801_CFLAGS}

bin_PROGRAMS = bwm801-user-template

bwm801_user_template_SOURCES = src/bwm801-user-template.cpp src/bwm801-user-template-gfx.cpp src/bwm801-user-template-UI.cpp
bwm801_user_template_LDADD = ${bwm801_LIBS}

