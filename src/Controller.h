/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler,

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#include "Model.h"
#include <SDL/SDL.h>
#include <string>
#include <sstream>
#include <map>

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

	map<string,double> qtable;

public:
	Controller(Model& m, bool* pKeepRunning);
	virtual ~Controller();

	void onChar(char c)
	{
		m_model.flap();
	}

	void onSpecialKey(int key)
	{
		m_model.flap();
	}

	void onMouseDown(int nButton, int x, int y)
	{
		m_model.flap();
	}

	void onMouseUp(int nButton, int x, int y)
	{
	}

	bool onMousePos(int x, int y)
	{
		return false;
	}

	void update();
	
	string getState()
	{
		int discretizer = 10;
		string s = "";

		// next tube state
		if(m_model.tubes.size() > 0){
		  int i = 0;
		  while(m_model.tubes[i]->x < m_model.bird.x){
			i++;
		  }
		  s += to_string(m_model.tubes[i]->up);
		  s += "," + to_string(m_model.tubes[i]->y / discretizer);
		  s += "," + to_string((m_model.tubes[i]->x - m_model.bird.x) / discretizer);
		}
		else s += "0,0," + to_string(400 / discretizer);

		// bird state
		s += "," + to_string(m_model.bird.y / discretizer);
		s += "," + to_string(m_model.bird.vert_vel / discretizer);

		return s;
	}
	
	double getQvalue(bool flap){
		string s;
		s = to_string(flap);
		s += "," + getState();
		map<string,double>::iterator it = qtable.find(s);
		double qvalue = 0.0;
		if(it != qtable.end())
			//qvalue = qtable.find(s)->second;
			qvalue = it->second;
		return qvalue;
	}

protected:
	void handleKeyPress(SDLKey eKey, SDLMod mod);
};


#endif // CONTROLLER_H
