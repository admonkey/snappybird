/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler, Jeff Puckett

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#include <stdio.h>
#include <stdlib.h>
#include <exception>
#include <iostream>
#include <vector>
#ifdef WINDOWS
#	include <windows.h>
#else
#	include <unistd.h>
#endif
#include "Model.h"
#include "View.h"
#include "Controller.h"

using std::cerr;
using std::cout;
using std::vector;


void mili_sleep(unsigned int nMiliseconds)
{
#ifdef WINDOWS
	MSG aMsg;
	while(PeekMessage(&aMsg, NULL, WM_NULL, WM_NULL, PM_REMOVE))
	{
		TranslateMessage(&aMsg);
		DispatchMessage(&aMsg);
	}
	SleepEx(nMiliseconds, 1);
#else
	nMiliseconds ? usleep(nMiliseconds*1024) : sched_yield(); // it is an error to sleep for more than 1,000,000
#endif
}



int main(int argc, char *argv[])
{
	int nRet = 0;
	try
	{
		bool keepRunning = true;
		bool keepFlying = true;
		bool sleep = true;
		map<string,double> mqtable;
		int game = 1;
		while(keepRunning){
			cout << game++ << "\n";
			keepFlying = true;
			Model *m;
			m = new Model;
			View *v;
			v = new View(*m, 500, 500);
			Controller *c;
			c = new Controller(*m, &keepRunning, &mqtable, &sleep);
			while(keepFlying && keepRunning)
			{
				if(!m->update())
					keepFlying = false;
				
				if(sleep) {
					v->update();
					mili_sleep(30);
				}
				c->update(keepFlying);
				//cout << c->getState() << "\n";
			}
			delete m;
			delete v;
			delete c;
			//mili_sleep(1000);
		}
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << "\n";
		nRet = 1;
	}

	return nRet;
}

