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
#include <time.h>

using std::cerr;
using std::cout;
using std::vector;

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string getDateTime(time_t time) {
    time_t     now = time;
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

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
		
		int totalFrames = 0;
		int games = 0;
		int highScore = 0;
		int bestGame = 0;
		int lastScore = 0;
		Json::Value scoreChanges;

		time_t start = time(0);
		while(keepRunning)
		{
			if(!m.update()){
				c.agent.died = true;
				totalFrames += m.frame;
				games++;
				if(m.score > highScore){
					 highScore = m.score;
					 bestGame = games;
				}
				if(m.score != lastScore){
				   if(m.score > 0)
					scoreChanges[to_str(games)] = m.score;
				   else
				   	scoreChanges[to_str(games)] = 0;
				}
				   if(m.score > 0)
					lastScore = m.score;
				   else
				   	lastScore = 0;
				lastScore = m.score;
				m.reset();
			} else	c.agent.died = false;
			
			if(c.viewOn){
				v.update();
				mili_sleep(30);
			}
			
			c.update();
		}
		time_t end = time(0);
		if(c.viewOn) mili_sleep(1000);

		InstanceSettingsJSON["StartTime"] = getDateTime(start);
		InstanceSettingsJSON["EndTime"] = getDateTime(end);
		InstanceSettingsJSON["TotalTimeSeconds"] = difftime(end, start);
		InstanceSettingsJSON["Frames"] = totalFrames;
		InstanceSettingsJSON["Games"] = games;
		InstanceSettingsJSON["HighScore"] = highScore;
		InstanceSettingsJSON["BestGame"] = bestGame;
		InstanceSettingsJSON["ScoreChanges"] = scoreChanges;
		
		//for (size_t i=0; i < scoreChanges.size(); i++)
			//InstanceSettingsJSON["BestGame"].append([scoreChanges[i].first] = scoreChanges[i].second);
		
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

