CFLAGS= -Wall
CFLAGS+=-I./include
CSTFLAGS =-DDEBUG

LIBS=
# LIBS+= -lmenu

SRC = $(wildcard ./src/*.c)

all: bin/clientTrain bin/clientTrainDEBUG 


bin/main: $(SRC) 
	@gcc $(CFLAGS) $^ -o $@ $(LIBS)

bin/mainDEBUG: $(SRC)
	@gcc $(CSTFLAGS) $(CFLAGS) $^ -o $@ $(LIBS)

bin/clientTrain: src/clientTrain.c
	@gcc $(CFLAGS) $^ -o $@ $(LIBS)

bin/clientTrainDEBUG: src/clientTrain.c
	@gcc $(CSTFLAGS) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	@rm -f ./bin/*







