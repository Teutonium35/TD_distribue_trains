CFLAGS= -Wall
CFLAGS+=-I./include
CSTFLAGS =-DDEBUG

LIBS=
# LIBS+= -lmenu

SRC = $(wildcard ./src/*.c)

all: bin/messagerieTrainApi bin/messagerieTrainApiDEBUG 


bin/main: $(SRC) 
	@gcc $(CFLAGS) $^ -o $@ $(LIBS)

bin/mainDEBUG: $(SRC)
	@gcc $(CSTFLAGS) $(CFLAGS) $^ -o $@ $(LIBS)

bin/messagerieTrainApi: src/messagerieTrainApi.c
	gcc $(CFLAGS) $^ -o $@ $(LIBS)

bin/messagerieTrainApiDEBUG: src/messagerieTrainApi.c
	gcc $(CSTFLAGS) $(CFLAGS) $^ -o $@ $(LIBS)

bin/messagerieTrainApiTEST: src/messagerieTrainApi.c
	@gcc $(CSTFLAGS) -DTEST $(CFLAGS) $^ -o $@ $(LIBS)

testMsg: bin/messagerieTrainApiTEST
		@./bin/messagerieTrainApiTEST

bin/receptionTestApiXway: src/receptionTestApiXway.c
	@gcc $(CSTFLAGS) -DTEST $(CFLAGS) $^ -o $@ $(LIBS)

testRecp: bin/receptionTestApiXway
	@./bin/receptionTestApiXway

clean:
	@rm -f ./bin/*







