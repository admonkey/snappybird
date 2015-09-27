/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler, Jeff Puckett

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#include "Controller.h"
#include "View.h"


Controller::Controller(Model& model, bool* pKeepRunning)
: m_model(model), m_pKeepRunning(pKeepRunning)
{
	initArrays();
}

Controller::Controller(Model& model, bool* pKeepRunning, Agent& agent)
: m_model(model), m_pKeepRunning(pKeepRunning)
{
	initArrays();
}

Controller::~Controller()
{
}

char g_shiftTable[] =
{
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	33, 34, 35, 36, 37, 38, '"', 40, 41, 42, 43,
	'<', '_', '>', '?',
	')', '!', '@', '#', '$', '%', '^', '&', '*', '(',
	58, ':', 60, 61, 62, 63, 64,
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'{', '|', '}', 94, 95, '~', 
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	123, 124, 125, 126, 127
};

char GSDL_shiftKey(char c)
{
	return g_shiftTable[c & 127];
}

char g_numPadTable[] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'.', '/', '*', '-', '+', 13, '='
};

char GSDL_filterKey(int key)
{
	if(key < 256)
		return (char)key;
	else if(key < 273)
		return g_numPadTable[key - 256];
	return '\0';
}

void Controller::initArrays()
{
	int n;
	for(n = 0; n < SDLK_LAST; n++)
		m_keyboard[n] = 0;
	m_mouse[1] = 0;
	m_mouse[2] = 0;
	m_mouse[3] = 0;
	m_mouseX = 0;
	m_mouseY = 0;
}

void Controller::handleKeyPress(SDLKey key, SDLMod mod)
{
	char cKey = GSDL_filterKey(key);
	if(cKey == 0)
	{
		onSpecialKey(key);
		return;
	}
	else
	{
		if(cKey == 27)
		{
			*m_pKeepRunning = false;
			return;
		}

		// Capitalize if shift is down
		if(mod & KMOD_SHIFT)
			cKey = GSDL_shiftKey(cKey);

		onChar(cKey);
	}
}

void Controller::onChar(char c)
{
	m_model.flap();
}

void Controller::update()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
				m_keyboard[event.key.keysym.sym] = 1;
				handleKeyPress(event.key.keysym.sym, event.key.keysym.mod);
				break;

			case SDL_KEYUP:
				m_keyboard[event.key.keysym.sym] = 0;
				break;

			case SDL_MOUSEBUTTONDOWN:
				m_mouse[event.button.button] = 1;
				m_mouseX = event.button.x;
				m_mouseY = event.button.y;
				onMouseDown(event.button.button, m_mouseX, m_mouseY);
				break;

			case SDL_MOUSEBUTTONUP:
				m_mouse[event.button.button] = 0;
				onMouseUp(event.button.button, m_mouseX, m_mouseY);
				break;

			case SDL_MOUSEMOTION:
				m_mouseX = event.motion.x;
				m_mouseY = event.motion.y;
				break;

			case SDL_QUIT:
				*m_pKeepRunning = false;
				break;

			default:
				break;
		}
	}
}


