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
#include <fstream>
#include <time.h>

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

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
		int game = 0;
		int score = 0;
		int highScore = score;
		int bestGame = game;
		while(keepRunning){
			game++;
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
					if(score < m->score){
						score = m->score;
						cout << to_string(score) << "\n";
					}
					
					mili_sleep(30);
				}
				c->update(keepFlying);
				//cout << c->getState() << "\n";
			}
			cout << "highScore: " << highScore << " bestGame: " << bestGame;
			cout << " score: " << m->score << " game: " << game << "\n";
			if(m->score > highScore){
				highScore = m->score;
				bestGame = game;
			}
			delete m;
			delete v;
			delete c;
			score = 0;
			//mili_sleep(1000);
		}
		cout << "writing qTable to file...\n";
		std::ofstream qfile;
		std::string fname = "qtable-hs" + to_string(highScore) + "-bg" + to_string(bestGame);
		fname += "-g" + to_string(game) + "-" + currentDateTime() + ".txt";
		qfile.open(fname.c_str(), std::ofstream::out);
		std::map<string,double>::iterator iter;
		std::string strToReturn = ""; //This is no longer on the heap
		for (iter = mqtable.begin(); iter != mqtable.end(); ++iter) {
		   strToReturn += iter->first; //Not a method call
		   strToReturn += "=";
		   strToReturn += to_string(iter->second);
		   strToReturn += ";";
		   // Make sure you don't modify table here or the iterators will not work as you expect
		}
		//...
		qfile << strToReturn;
		qfile.close();
		cout << "qTable written to file.\n";
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << "\n";
		nRet = 1;
	}

	return nRet;
}

