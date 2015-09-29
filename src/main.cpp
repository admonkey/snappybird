/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler,

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
#include "json/json.h"
#include <fstream>

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
		Json::Value InstanceSettingsJSON;
		InstanceSettingsJSON["InstanceID"] = argv[1];
		InstanceSettingsJSON["Hostname"] = argv[2];
      
		bool keepRunning = true;
		Model m;
		View v(m, 500, 500);
		Controller c(m, &keepRunning);
		while(keepRunning)
		{
			if(!m.update()){
				c.agent.died = true;
				m.reset();
			} else	c.agent.died = false;
			
			if(c.viewOn){
				v.update();
				mili_sleep(30);
			}
			
			c.update();
		}
		if(c.viewOn) mili_sleep(1000);

		InstanceSettingsJSON["StateSettings"] = c.agent.state.StateSettingsJSON;
		Json::StyledWriter styledWriter;
		std::cout << styledWriter.write(InstanceSettingsJSON);
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << "\n";
		nRet = 1;
	}

	return nRet;
}

