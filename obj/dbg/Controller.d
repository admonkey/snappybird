../obj/dbg/Controller.o: Controller.cpp Controller.h Model.h rand.h View.h image.h
	g++ -Wall -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT -g -D_DEBUG -c Controller.cpp -o ../obj/dbg/Controller.o
