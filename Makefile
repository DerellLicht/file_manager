SHELL=cmd.exe
USE_DEBUG = NO

ifeq ($(USE_DEBUG),YES)
CFLAGS=-Wall -O -g
LFLAGS=
else
CFLAGS=-Wall -O3
LFLAGS=-s
endif
CFLAGS += -Wno-write-strings
CFLAGS += -Weffc++

# link application-specific sources
CSRC+=read_file.cpp DFileReadText.cpp DFileReadBinary.cpp common_funcs.cpp 

OBJS = $(CSRC:.cpp=.o)

BIN=read_file

#************************************************************
%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@

all: $(BIN).exe

clean:
	rm -f *.exe *.zip *.bak $(OBJS)

lint:
	cmd /C "/lint9/lint-nt +v -width(160,4) $(LiFLAGS) -i/lint9 mingw.lnt -os(_lint.tmp) lintdefs.cpp $(CSRC)"

depend:
	makedepend $(CFLAGS) $(CSRC)

#************************************************************

$(BIN).exe: $(OBJS)
	g++ $(CFLAGS) -s $(OBJS) -o $@

# DO NOT DELETE

read_file.o: common.h DFileRead.h
DFileReadText.o: common.h DFileRead.h
DFileReadBinary.o: common.h DFileRead.h
common_funcs.o: common.h
