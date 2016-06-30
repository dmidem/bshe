SRC=src/bshe.cpp src/Batch.cpp src/FileSystem.cpp src/DirectoryIterator.cpp
OBJ=$(SRC:.cpp=.o)
EXE=bshe

CC=g++
CFLAGS=-Wall -O3 -g
#LDFLAGS=-static-libgcc -static-libstdc++ -static
RM=rm

all: $(EXE)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

$(EXE): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

clean:
	$(RM) $(OBJ) $(EXE)

depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $(SRC) >> ./.depend

-include .depend

.PHONY: all clean
