/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler,

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#ifndef VIEW_H
#define VIEW_H

#include "image.h"
#include <SDL/SDL.h>


class Model;


class Rect
{
public:
	int x, y, w, h;

	Rect()
	{
	}

	Rect(int _x, int _y, int _w, int _h)
	{
		x = _x;
		y = _y;
		w = _w;
		h = _h;
	}
};


class View
{
protected:
	SDL_Surface* m_pScreen;
	Rect m_screenRect;
	Model& model;
	GImage image_bird1;
	GImage image_bird2;
	GImage image_tube_up;
	GImage image_tube_down;

	GImage* m_pImage;

public:
	View(Model& m, int w, int h);
	virtual ~View();
	void update();
	static void blitImage(SDL_Surface* pScreen, int x, int y, GImage* pImage);
	SDL_Surface* makeScreen(int x, int y);
};



#endif // VIEW_H
