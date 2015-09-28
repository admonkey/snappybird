/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler, Jeff Puckett

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#include "Model.h"
#include <SDL/SDL.h>
#include "Agent.h"
#include <iostream>

using std::vector;

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
	
	Agent agent;
	bool viewOn;

public:
	Controller(Model& m, bool* pKeepRunning);
	virtual ~Controller();

	void onChar(char c);

	void onSpecialKey(int key)
	{
		onChar(' ');
	}

	void onMouseDown(int nButton, int x, int y)
	{
		onChar(' ');
	}

	void onMouseUp(int nButton, int x, int y)
	{
	}

	bool onMousePos(int x, int y)
	{
		return false;
	}

	void update();

protected:
	void handleKeyPress(SDLKey eKey, SDLMod mod);
	void initArrays();
};


#endif // CONTROLLER_H
