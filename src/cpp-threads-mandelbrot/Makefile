ifdef MANDEL_NO_DISPLAY
	DIS=-DNO_DISPLAY
else
	DIS=
endif

CC                   = gcc
CXX                  = g++
LINK_OPT             =
VERSION              =
OPTIMIZE_FLAGS       = -O3 -finline-functions
CXXFLAGS             = -std=c++1y -DNODBG -DNO_CMAKE_CONFIG -Wall
CFLAGS               = $(DIS)
LDFLAGS              = $(DIS) -L/usr/X11R6/lib -L.
INCS                 = -I build/libraries/
LIBS                 = -lmarX2 -lX11 -lm
INCLUDES             = -I. $(INCS) -I/usr/X11R6/include
TARGET               = seq farm map pipeline_with_map
SOURCES			 =  mandel.cpp mandel_farm.cpp mandel_map.cpp mandel_pipeline_with_map.cpp
SRC = .
INC = .

.PHONY: all clean cleanall

all:$(TARGET)

libmarX2.a: $(SRC)/marX2.c $(INC)/marX2.h
	gcc -c $(OPTIMIZE_FLAGS) $(CFLAGS) -I/usr/X11R6/include $(INCLUDES) $(SRC)/marX2.c
	ar -rv libmarX2.a marX2.o
	ranlib libmarX2.a

seq: mandel.cpp libmarX2.a
	$(CXX)  $(CXXFLAGS) $(OPTIMIZE_FLAGS)  $(CFLAGS) $(INCLUDES) $(LDFLAGS) -c -o seq.o mandel.cpp $(LIBS)
	$(CXX)  $(CXXFLAGS) $(OPTIMIZE_FLAGS)  $(CFLAGS) $(INCLUDES) $(LDFLAGS)  -o seq seq.o $(LIBS)

farm:
	$(CXX)  $(CXXFLAGS) $(OPTIMIZE_FLAGS)  $(CFLAGS) $(INCLUDES) $(LDFLAGS) -c -o farm.o mandel_farm.cpp $(LIBS) -pthread
	$(CXX)  $(CXXFLAGS) $(OPTIMIZE_FLAGS)  $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o farm farm.o $(LIBS) -pthread

map:
	$(CXX)  $(CXXFLAGS) $(OPTIMIZE_FLAGS)  $(CFLAGS) $(INCLUDES) $(LDFLAGS) -c -o map.o mandel_map.cpp $(LIBS) -pthread
	$(CXX)  $(CXXFLAGS) $(OPTIMIZE_FLAGS)  $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o map map.o $(LIBS) -pthread

thread_pool:
	$(CXX)  $(CXXFLAGS) $(OPTIMIZE_FLAGS)  $(CFLAGS) $(INCLUDES) $(LDFLAGS) -c -o mandel_thread_pool.o mandel_thread_pool.cpp $(LIBS) -pthread
	$(CXX)  $(CXXFLAGS) $(OPTIMIZE_FLAGS)  $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o mandel_thread_pool mandel_thread_pool.o $(LIBS) -pthread

pipeline_with_map:
	$(CXX)  $(CXXFLAGS) $(OPTIMIZE_FLAGS)  $(CFLAGS) $(INCLUDES) $(LDFLAGS) -c -o pipeline_with_map.o mandel_pipeline_with_map.cpp $(LIBS) -pthread
	$(CXX)  $(CXXFLAGS) $(OPTIMIZE_FLAGS)  $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o pipeline_with_map pipeline_with_map.o $(LIBS) -pthread


clean:
	rm -f libmarX2.a *.o

cleanall: clean
	rm -fr $(TARGET) *~ *.dSYM

depend:
	makedepend $(SOURCES)


# DO NOT DELETE THIS LINE -- make depend depends on it.

mandel.o: /usr/include/stdio.h /usr/include/_types.h
mandel.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
mandel.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
mandel.o: marX2.h /usr/include/X11/Xlib.h /usr/include/sys/types.h
mandel.o: /usr/include/sys/appleapiopts.h /usr/include/machine/types.h
mandel.o: /usr/include/i386/types.h /usr/include/machine/endian.h
mandel.o: /usr/include/i386/endian.h /usr/include/sys/_endian.h
mandel.o: /usr/include/libkern/_OSByteOrder.h
mandel.o: /usr/include/libkern/i386/_OSByteOrder.h
mandel.o: /usr/include/sys/_structs.h /usr/include/X11/X.h
mandel.o: /usr/include/X11/Xfuncproto.h /usr/include/X11/Xosdefs.h
mandel.o: /usr/include/stddef.h /usr/include/X11/Xutil.h
mandel.o: /usr/include/X11/keysym.h /usr/include/X11/keysymdef.h
mandel.o: /usr/include/stdlib.h /usr/include/available.h
mandel.o: /usr/include/sys/wait.h /usr/include/sys/signal.h
mandel.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
mandel.o: /usr/include/i386/_structs.h /usr/include/sys/resource.h
mandel.o: /usr/include/alloca.h /usr/include/string.h /usr/include/math.h
mandel.o: /usr/include/architecture/i386/math.h /usr/include/sys/time.h
mandel.o: /usr/include/time.h /usr/include/_structs.h
mandel.o: /usr/include/sys/_select.h
marX2.o: marX2.h /usr/include/X11/Xlib.h /usr/include/sys/types.h
marX2.o: /usr/include/sys/appleapiopts.h /usr/include/sys/cdefs.h
marX2.o: /usr/include/machine/types.h /usr/include/i386/types.h
marX2.o: /usr/include/i386/_types.h /usr/include/sys/_types.h
marX2.o: /usr/include/machine/_types.h /usr/include/machine/endian.h
marX2.o: /usr/include/i386/endian.h /usr/include/sys/_endian.h
marX2.o: /usr/include/libkern/_OSByteOrder.h
marX2.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/sys/_structs.h
marX2.o: /usr/include/X11/X.h /usr/include/X11/Xfuncproto.h
marX2.o: /usr/include/X11/Xosdefs.h /usr/include/stddef.h
marX2.o: /usr/include/_types.h /usr/include/X11/Xutil.h
marX2.o: /usr/include/X11/keysym.h /usr/include/X11/keysymdef.h
marX2.o: /usr/include/stdio.h /usr/include/stdlib.h /usr/include/available.h
marX2.o: /usr/include/sys/wait.h /usr/include/sys/signal.h
marX2.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
marX2.o: /usr/include/i386/_structs.h /usr/include/sys/resource.h
marX2.o: /usr/include/alloca.h /usr/include/string.h /usr/include/math.h
marX2.o: /usr/include/architecture/i386/math.h
