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
#include "json/json.h"
#include <fstream>
#include <time.h>
#include <sys/stat.h>

using std::cerr;
using std::cout;
using std::vector;

bool fileExists(const char* fname)
{
	struct stat buf;
	if (stat(fname, &buf) != -1) {
		cout << "found " << fname << "...\n";
		return true;
	}
	else cout << fname << " doesn't exist.\n";
	return false;
}

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
		Json::StyledWriter styledWriter;
		Json::Value InstanceSettingsJSON;
		InstanceSettingsJSON["InstanceID"] = argv[1];
		InstanceSettingsJSON["Hostname"] = argv[2];

		int totalFrames = 0;
		int totalGames = 0;
		int totalHighScore = 0;
		int totalBestGame = 0;
		int totalScoreChangeCount = 0;

		// import instance
		Json::Value ImportSettingsJSON;
		if (fileExists("Settings.json"))
		{
			std::ifstream ImportSettings("Settings.json", std::ifstream::binary);
			ImportSettings >> ImportSettingsJSON;
			//std::cout << styledWriter.write(ImportSettingsJSON);
			cout << "Settings.json imported.\n";

			// total scores
			totalFrames = ImportSettingsJSON["TotalScores"]["TotalFrames"].asInt();
			totalGames = ImportSettingsJSON["TotalScores"]["TotalGames"].asInt();
			totalHighScore = ImportSettingsJSON["TotalScores"]["TotalHighScore"].asInt();
			totalBestGame = ImportSettingsJSON["TotalScores"]["TotalBestGame"].asInt();
			totalScoreChangeCount = ImportSettingsJSON["TotalScores"]["TotalScoreChangeCount"].asInt();
		}

		bool keepRunning = true;
		Model m;
		View v(m, 500, 500);
		Controller c(m, &keepRunning, ImportSettingsJSON);
		c.agent.qt.readTable("");

		int frames = 0;
		int games = 0;
		int highScore = 0;
		int bestGame = 0;
		int lastScore = 0;
		//Json::Value scoreChangesJSON;
		std::string scoreChangesCSV = "(0,0)";
		int scoreChangeCount = 0;

		time_t start = time(0);
		while(keepRunning)
		{
			// if bird dies, update stats & reset game
			if(!m.update()){
				c.agent.died = true;
				
				frames += m.frame;
				totalGames++;
				games++;
				// high score
				if(m.score > highScore){
					 highScore = m.score;
					 bestGame = games;
				}
				// last score
				// check > 0, else null comparison causes blank entry
				if(m.score != lastScore){
				   scoreChangeCount++;
				   if(m.score > 0)
					//scoreChangesJSON[to_str(games)] = m.score;
					scoreChangesCSV += ",(" + to_str(totalGames) + "," + to_str(m.score) + ")";
				   else
				   	//scoreChangesJSON[to_str(games)] = 0;
				   	scoreChangesCSV += ",(" + to_str(totalGames) + ",0)";
				}
				   if(m.score > 0)
					lastScore = m.score;
				   else
				   	lastScore = 0;
				lastScore = m.score;

				m.reset();
				if( (c.viewOn) || (games % 10 == 0) ){
					cout << "totalGames: " << totalGames;
					cout << " totalHighScore: " << totalHighScore << " totalBestGame: " << totalBestGame;
					cout << " highScore: " << highScore << " bestGame: " << bestGame;
					cout << " lastScore: " << lastScore << " games: " << games << "\n";
				}
			} else	c.agent.died = false; // signal agent reward system
			
			// if watching live
			if(c.viewOn){
				v.update();
				mili_sleep(30);
			}

			c.update();
			if(c.agent.died)
				c.agent.previousState = "";
		}
		time_t end = time(0);
		if(c.viewOn) mili_sleep(1000);
		


		// update settings object
		InstanceSettingsJSON["AgentSettings"] = c.agent.AgentSettingsJSON;
		InstanceSettingsJSON["StateSettings"] = c.agent.state.StateSettingsJSON;
		InstanceSettingsJSON["Time"]["StartTime"] = getDateTime(start);
		InstanceSettingsJSON["Time"]["EndTime"] = getDateTime(end);
		InstanceSettingsJSON["Time"]["TotalTimeSeconds"] = difftime(end, start);

		// instance scores
		InstanceSettingsJSON["InstanceScores"]["Frames"] = frames;
		InstanceSettingsJSON["InstanceScores"]["Games"] = games;
		InstanceSettingsJSON["InstanceScores"]["ScoreChangeCount"] = scoreChangeCount;
		InstanceSettingsJSON["InstanceScores"]["HighScore"] = highScore;
		InstanceSettingsJSON["InstanceScores"]["BestGame"] = bestGame;

		// total scores
		InstanceSettingsJSON["TotalScores"]["TotalFrames"] = totalFrames + frames;
		InstanceSettingsJSON["TotalScores"]["TotalGames"] = totalGames + games;
		InstanceSettingsJSON["TotalScores"]["TotalScoreChangeCount"] = totalScoreChangeCount + scoreChangeCount;
		if(highScore > totalHighScore){
			InstanceSettingsJSON["TotalScores"]["TotalHighScore"] = highScore;
			InstanceSettingsJSON["TotalScores"]["TotalBestGame"] = totalGames + bestGame;
		}
		else {
			InstanceSettingsJSON["TotalScores"]["TotalHighScore"] = totalHighScore;
			InstanceSettingsJSON["TotalScores"]["TotalBestGame"] = totalBestGame;
		}

		// write out results & settings
		std::cout << styledWriter.write(InstanceSettingsJSON);
		std::ofstream gSettings("Settings.json", std::ofstream::binary);
		gSettings << styledWriter.write(InstanceSettingsJSON);
		
		// write out score changes CSV
		std::ofstream gChanges("ScoreChanges.csv", std::ofstream::binary);
		gChanges << scoreChangesCSV;

		// write out qTable
		c.agent.qt.writeTable("");
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << "\n";
		nRet = 1;
	}

	return nRet;
}

