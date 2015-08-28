../obj/opt/main.o: main.cpp Model.h rand.h View.h image.h Controller.h
	g++ -Wall -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT -O3 -c main.cpp -o ../obj/opt/main.o
