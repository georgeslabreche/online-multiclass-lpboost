# This is a make file for OMCBoost package
# Amir Saffari, amir@ymer.org
#-----------------------------------------

# Compiler options
CC = g++
#CC = /usr/bin/arm-linux-gnueabihf-g++

INCLUDEPATH = -I/usr/local/include -I../eigen -I$(HOME)/local/include
LINKPATH = -L/usr/local/lib -L$(HOME)/local/lib

# OPTIMIZED
# Compile for gcc development environment:
CFLAGS = -c -O3 -Wall -march=native -mtune=native -DNDEBUG -Wno-deprecated

# Compile for ARM gcc target environment:
#CFLAGS = -c -O3 -Wall -DNDEBUG -Wno-deprecated

# libconfig++ depedency.
LDFLAGS = -lconfig++ -lboost_serialization

# Source directory and files
SOURCEDIR = src
HEADERS := $(wildcard $(SOURCEDIR)/*.h)
SOURCES := $(wildcard $(SOURCEDIR)/*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
LIBLARANKDIR := $(SOURCEDIR)/linear_larank

# Target output
BUILDTARGET = OMCBoost
#BUILDTARGET = OrbitAI_Boost

# Build
all: $(BUILDTARGET)
$(BUILDTARGET): $(OBJECTS) $(SOURCES) $(HEADERS) $(LIBLARANKDIR)/LaRank.o $(LIBLARANKDIR)/vectors.o 
	$(CC) $(LINKPATH) $(OBJECTS) $(LIBLARANKDIR)/LaRank.o $(LIBLARANKDIR)/vectors.o -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDEPATH) $< -o $@

$(LIBLARANKDIR)/LaRank.o: $(LIBLARANKDIR)/LaRank.cpp  $(LIBLARANKDIR)/LaRank.h $(LIBLARANKDIR)/vectors.o
	$(CC) $(CFLAGS) $(INCLUDEPATH) -o $(LIBLARANKDIR)/LaRank.o $(LIBLARANKDIR)/LaRank.cpp

$(LIBLARANKDIR)/vectors.o: $(LIBLARANKDIR)/vectors.h $(LIBLARANKDIR)/wrapper.h
	$(CC) $(CFLAGS) $(INCLUDEPATH) -o $(LIBLARANKDIR)/vectors.o $(LIBLARANKDIR)/vectors.cpp

clean:
	rm -f $(SOURCEDIR)/*~ $(SOURCEDIR)/*.o
	rm -f $(LIBLARANKDIR)/*.o
	rm -f $(BUILDTARGET)
