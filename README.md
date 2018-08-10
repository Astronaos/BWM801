# BWM801 : multi-threaded cross-platform application engine
Brian W. Mulligan (mailto:astrobit2@yahoo.com)

This is version 1.0


# Linux:
## prerequisites:
- gnu-c++
- X11 (libX11-devel) # window system
- Mesa (mesa-libGL-devel and mesa-libGLU-devel) # OpenGL library
- mesa GLU () # gl utilities library
- freetype (freetype-devel) : font library
- ftgl (ftgl-devel) : API to use freetype with opengl
- libpng (libpng-devel) : API for reading and writing PNG files.


## preparing for configure:
```
mkdir m4
libtoolize
aclocal
autoconf
automake --add-missing
autoreconf
```

## configuring: 
NOTE: you can use general standard configure options. e.g. use --prefix=<path> to define the installation path
```
     ./configure
```

## compiling & install:
```
make 
make install
```

## running
- make sure that you are in the directory where your user shared library has been created
- make sure that libbwm801.la is in your LD_LIBRARY_PATH or the current directory
- make sure that bwm801-launcher is in your PATH
```
bwm801-launcher
```

# Windows (using Visual Studio)

##  compiling:
- Open solution (BWM801.sln) in Visual Studio
- Build

## running
- Place bwm801-launcher.exe, bwm801.dll, your user dll (e.g. bwm801-user-template.dll) in a single folder
- Any data files in that folder or approiate locations
- Lauch bwm801-launcher.exe
