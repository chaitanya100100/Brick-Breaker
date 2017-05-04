FLAG = -lGL -lglfw -ldl -I"include" -L"/usr/lib" -pthread

all: a.out

a.out: main.o noedit.o block.o brick.o canon.o bullet.o bucket.o controls.o mirror.o score.o collision.o
	g++ main.o noedit.o block.o brick.o canon.o bullet.o bucket.o controls.o mirror.o score.o collision.o glad.c $(FLAG) ./libIrrKlang.so 

main.o: main.cpp main.h glad.c
	g++ -c main.cpp glad.c $(FLAG)

noedit.o: noedit.cpp noedit.h glad.c
	g++ -c noedit.cpp glad.c $(FLAG)

controls.o: controls.cpp controls.h glad.c
	g++ -c controls.cpp glad.c $(FLAG)

block.o: block.cpp block.h
	g++ -c block.cpp $(FLAG)

brick.o: brick.cpp brick.h
	g++ -c brick.cpp $(FLAG)

canon.o: canon.cpp canon.h
	g++ -c canon.cpp $(FLAG)

bullet.o: bullet.cpp bullet.h
	g++ -c bullet.cpp $(FLAG)

bucket.o: bucket.cpp bucket.h
	g++ -c bucket.cpp $(FLAG)

mirror.o: mirror.cpp mirror.h
	g++ -c mirror.cpp $(FLAG)

score.o: score.cpp score.h
	g++ -c score.cpp $(FLAG)

collision.o: collision.cpp
	g++ -c collision.cpp $(FLAG)

clean:
	rm a.out main.o noedit.o block.o brick.o canon.o bullet.o bucket.o glad.o controls.o mirror.o collision.o score.o
