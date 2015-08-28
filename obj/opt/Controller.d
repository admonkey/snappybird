../obj/opt/Controller.o: Controller.cpp Controller.h Model.h rand.h View.h image.h
	g++ -Wall -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT -O3 -c Controller.cpp -o ../obj/opt/Controller.o
