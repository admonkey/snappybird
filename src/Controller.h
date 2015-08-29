/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler & Jeff Puckett <jeff@jeffpuckett.com>

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#include "Model.h"
#include <SDL/SDL.h>

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

	// added for automation
	clock_t sec;
	clock_t stt;

	int m_keyboard[SDLK_LAST];
	int m_mouse[4];
	int m_mouseX;
	int m_mouseY;
	bool* m_pKeepRunning;

public:
	Controller(Model& m, bool* pKeepRunning);
	virtual ~Controller();

	// added for automation
	void flap()
	{
		m_model.flap();
	}

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

protected:
	void handleKeyPress(SDLKey eKey, SDLMod mod);
};


#endif // CONTROLLER_H
