/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler, Jeff Puckett

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#include "Model.h"
#include <SDL/SDL.h>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include "rand.h"
#include <time.h>

template <typename T>
std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

using std::vector;
using std::string;
using std::map;
using std::pair;

class View;


#ifndef CONTROLLER_H
#define CONTROLLER_H


class Controller
{
public:
	Model& m_model;
	int ttl;
	int frame;
	int m_keyboard[SDLK_LAST];
	int m_mouse[4];
	int m_mouseX;
	int m_mouseY;
	bool* m_pKeepRunning;

	map<string,double>* qtable;
	string previousState;
	bool previousFlap;
	bool* ssleep;
	bool DEBUGZ;
	int* ghighScore;
	bool skip;
	bool agentPlay;
	int rapidFlap;
	int discretizer;
	std::string settings;
	Rand rand;
	int explorationRate;
	double discount;

public:
	Controller(Model& m, bool* pKeepRunning, map<string,double>* mqtable, bool* sleep, int* highScore, int randseed);
	virtual ~Controller();

	void onChar(char c)
	{
		if(c == 's'){
			std::cout << "skipping " << (*ghighScore - m_model.score) << " pipes..\n";
			skip = true;
			*ssleep = false;
			return;
		}
		if(c == 'a'){
			
			if(agentPlay){
				agentPlay = false;
				std::cout << "Disabling agent control.\n";
			} else {
				agentPlay = true;
				std::cout << "Enabling agent control.\n";
			}
			return;
		}
		if(c == 'r'){
			rapidFlap = 30;
			return;
		}
		m_model.flap();
		previousFlap = true;
	}

	void onSpecialKey(int key)
	{
		m_model.flap();
		previousFlap = true;
	}

	void onMouseDown(int nButton, int x, int y)
	{
		//m_model.flap();
		if(*ssleep) *ssleep = false;
		else *ssleep = true;
	}

	void onMouseUp(int nButton, int x, int y)
	{
	}

	bool onMousePos(int x, int y)
	{
		return false;
	}

	void update(bool keepFlying);
	
	string getState()
	{
		
		string s = "";

		// next tube state
		if(m_model.tubes.size() > 0){
		  int i = 0;
		  while(m_model.tubes[i]->x < m_model.bird.x){
			i++;
		  }
		  s += to_string(m_model.tubes[i]->up);
		  /*if(m_model.tubes[i]->up){
		  	s += to_string((m_model.bird.y - m_model.tubes[i]->y) / discretizer);
		  }else	s += to_string((m_model.tubes[i]->y - m_model.bird.y) / discretizer);*/
		  s += "," + to_string((m_model.bird.y - m_model.tubes[i]->y) / discretizer);
		  s += "," + to_string((m_model.tubes[i]->x - m_model.bird.x) / discretizer);
		  
		//  std::cout << "m_model.tubes[i]->y = " << m_model.tubes[i]->y << "\n";
		}
		else s += "0,0," + to_string(400 / discretizer);

		// bird state
		//s += "," + to_string(m_model.bird.y / discretizer);
		s += "," + to_string(m_model.bird.vert_vel / discretizer);
		//std::cout << s << "\n";
		return s;
	}
	
	double getQvalue(bool flap, string state){
		string s;
		s = to_string(flap);
		s += "," + state;
		map<string,double>::iterator it = qtable->find(s);
		double qvalue = 0.0;
		if(it != qtable->end())
			//qvalue = qtable.find(s)->second;
			qvalue = it->second;
		return qvalue;
	}

	void setQvalue(bool flap, bool reward){
		// combine last action with previous state and current reward
		string s;
		s = to_string(flap);
		s += "," + previousState;
		double qvalue = 0.0;
		if(reward)
			qvalue = 1.0;
		//if(reward && !flap)
			//qvalue = 0.01;
		if(!reward)
			qvalue = -1000.0;
		// Broken-Down Q-Learning Formula
		//double q_j_flap = getQvalue(true, getState());
		//double q_j_noflap = getQvalue(false, getState());
		
		//qvalue += (discount * max(q_j_flap, q_j_noflap));
		qvalue += (discount * std::max(getQvalue(true, getState()), getQvalue(false, getState())));
		//qtable->erase(std::pair<string,double>(s, qvalue));
		map<string,double>::iterator it = qtable->find(s);
		if(it != qtable->end())
			it->second = qvalue;
		else
			qtable->insert(std::pair<string,double>(s, qvalue));
		if(*ssleep) std::cout << "\t setq: " << qvalue << " = " << s << "\n";
	}
	
	bool explore(){
		return (rand.next(explorationRate) == 0); 
	}
	
	bool toFlapOrNotToFlap(){
		return (rand.next(20) == 0); // 5% flap rate = 1/20 chance
	}

protected:
	void handleKeyPress(SDLKey eKey, SDLMod mod);
};


#endif // CONTROLLER_H
