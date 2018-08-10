# BWM801 : multi-threaded cross-platform application engine
# Brian W. Mulligan (mailto:astrobit2@yahoo.com)

# This is version 1.0


# compiling under Linux:
#
#	prerequisites:	gnu-c++
#						X11 (libX11-devel) # window system
#						Mesa (mesa-libGL-devel and mesa-libGLU-devel) # OpenGL library
#						mesa GLU () # gl utilities library
#						freetype (freetype-devel) : font library
#						ftgl (ftgl-devel) : API to use freetype with opengl
#           libpng (libpng-devel) : API for reading and writing PNG files.
#
#   preparing for configure:
#     mkdir m4
#     libtoolize
#     aclocal
#     autoconf
#     automake --add-missing
#     autoreconf
#
#   configuring: NOTE: you can use general standard configure options. e.g. use --prefix=<path> to define the installation path
#     ./configure
#
#   compiling:
#		  make 
#
#   installing:
#     make install
#
#
# compiling Windows executable with Visual Studio
#
#   compiling:
#		Open solution (BWM801.sln) in Visual Studio
#		Build
#
#

