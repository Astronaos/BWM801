all: dirtest coreLinux 
#all: dirtest coreWin coreLinux

LIBDIR=./lib
BINDIR=./bin
OBJDIR=./obj
WOBJDIR=./w64obj
SRCDIR=./src
INCDIR=./include
CFLAGS=-I$(INCDIR) -I/usr/include/freetype2 -std=c++11 -c
WCFLAGS=-I$(INCDIR) -std=c++11 -m64 -c
LFLAGS=-std=c++11
WLFLAGS=-std=c++11 -static -m64
LIBS=-lfreetype -lftgl -lX11 -lGLU -lGL
WLIBS= -lopengl32 -lglu32 -lgdi32
WCC=x86_64-w64-mingw32-g++
CC=g++
CORE_INCLUDES=$(wildcard $(INCDIR)/core*.hpp)
CORE_SRC=$(wildcard $(SRCDIR)/core*.cpp)
CORE_OBJ=$(subst $(SRCDIR),$(OBJDIR),$(patsubst %.cpp,%.o,$(CORE_SRC)))
WIN_CORE_OBJ=$(subst $(SRCDIR),$(WOBJDIR),$(patsubst %.cpp,%.wo,$(CORE_SRC)))

clean:
	-rm $(OBJDIR)/*.o
	-rm $(WOBJDIR)/*.wo
	-rm $(LIBDIR)/*.a
	-rm $(LIBDIR)/*.wa

#dirtest: make directories $(OBJDIR) and $(WOBJDIR) in case they don't already exist
dirtest:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(WOBJDIR)
	@mkdir -p $(LIBDIR)
	@mkdir -p $(BINDIR)

$(WOBJDIR)/%.wo: $(SRCDIR)/%.cpp $(CORE_INCLUDES)
	$(WCC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(CORE_INCLUDES)
	$(CC) $(CFLAGS) $< -o $@

$(LIBDIR)/core.a: $(CORE_OBJ) $(OBJDIR)/Xwin_main.o
	-rm $(LIBDIR)/libEngineMk1.a
	ar -cvq  $(LIBDIR)/libEngineMk1.a $(CORE_OBJ) $(OBJDIR)/Xwin_main.o

coreWin: $(WIN_CORE_OBJ) $(WOBJDIR)/winmain.wo
coreLinux: $(CORE_OBJ) $(OBJDIR)/Xwin_main.o $(LIBDIR)/core.a
