/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler,

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#include "View.h"
#include "Model.h"
#include "error.h"


View::View(Model& m, int w, int h)
: model(m)
{
	m_pScreen = makeScreen(w, h);
	m_screenRect.x = 0;
	m_screenRect.y = 0;
	m_screenRect.w = m_pScreen->w;
	m_screenRect.h = m_pScreen->h;

	m_pImage = new GImage();
	m_pImage->resize(500, 500);

	image_bird1.loadPng("bird1.png");
	image_bird2.loadPng("bird2.png");
	image_tube_up.loadPng("tube_up.png");
	image_tube_down.loadPng("tube_down.png");
}

// virtual
View::~View()
{
	delete(m_pImage);
}

void View::update()
{
	// Draw the sky
	m_pImage->clear(0xff00ffff);

	// Draw the bird
	Bird& bird = model.bird;
	if(bird.time_since_flap < 4)
		m_pImage->blit(bird.x, bird.y, &image_bird2);
	else
		m_pImage->blit(bird.x, bird.y, &image_bird1);

	// Draw the tubes
	for(size_t i = 0; i < model.tubes.size(); i++) {
		Tube* t = model.tubes[i];
		if(t->up)
			m_pImage->blit(t->x, t->y, &image_tube_up);
		else
			m_pImage->blit(t->x, t->y - 400, &image_tube_down);
	}



	// Lock the screen for direct access to the pixels
	SDL_Surface *pScreen = m_pScreen;
	if ( SDL_MUSTLOCK(pScreen) )
	{
		if ( SDL_LockSurface(pScreen) < 0 )
			throw Ex("Failed to lock the SDL surface");
	}

	// Clear the screen
	SDL_FillRect(pScreen, NULL/*&r*/, 0x000000);

	// Draw the dialog
	blitImage(pScreen, m_screenRect.x, m_screenRect.y, m_pImage);

	// Unlock the screen
	if ( SDL_MUSTLOCK(pScreen) )
		SDL_UnlockSurface(pScreen);

	// Update the whole screen
	SDL_UpdateRect(pScreen, m_screenRect.x, m_screenRect.y, m_screenRect.w, m_screenRect.h);

}

SDL_Surface* View::makeScreen(int x, int y)
{
	unsigned int flags =
//		SDL_FULLSCREEN |
//		SDL_HWSURFACE |
		SDL_SWSURFACE |
//		SDL_DOUBLEBUF |
		SDL_ANYFORMAT;
	SDL_Surface* pScreen = SDL_SetVideoMode(x, y, 32, flags);
	if(!pScreen)
	{
		// SDL_GetError();
		throw "failed to create SDL screen";
	}
	return pScreen;
}

inline Uint32* getPixMem32(SDL_Surface *surface, int x, int y)
{
	return (Uint32*)((Uint8*)surface->pixels + y * surface->pitch + (x << 2));
}

// static
void View::blitImage(SDL_Surface* pScreen, int x, int y, GImage* pImage)
{
	if(pScreen->format->BytesPerPixel == 4)
	{
		unsigned int* pRGB = pImage->m_pPixels;
		int w = pImage->width();
		int h = pImage->height();
		int yy;
		Uint32* pPix;
		for(yy = 0; yy < h; yy++)
		{
			pPix = getPixMem32(pScreen, x, y);
#ifdef DARWIN
			unsigned int* pRaw = &pRGB[yy * w];
			for(size_t i = 0; i < w; i++)
				*(pPix++) = ntohl(*(pRaw++));
#else
			memcpy(pPix, &pRGB[yy * w], w * sizeof(unsigned int));
#endif
			y++;
		}
	}
	else
		throw Ex("Unsupported number of bytes per pixel");
}

