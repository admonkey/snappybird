/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler,

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#ifndef MODEL_H
#define MODEL_H

#include "rand.h"
#include <vector>

using std::vector;

class Model;



class Bird
{
public:
	int x;
	int y;
	double vert_vel;
	int time_since_flap;

	Bird() {
		x = 100;
		y = 100;
		vert_vel = 0.0;
	}

	bool update() {
		time_since_flap++;
		y += vert_vel;
		vert_vel += 1.5;
		if(y < -55)
			return false;
		else if(y > 500)
			return false;
		return true;
	}

	void flap() {
		vert_vel = -12.0;
		if(time_since_flap > 4)
			time_since_flap = 0;
		else
			time_since_flap = 5;
	}
};


class Tube
{
public:
	int x;
	int y;
	bool up;

	Tube(int _y, bool _up) {
		y = _y;
		x = 500;
		up = _up;
	}

	bool update(Bird& bird) {
		x -= 5;
		if(x < bird.x + 60 && x + 45 > bird.x) {
			if(up) {
				if(bird.y + 50 > y)
					return false;
			} else {
				if(bird.y + 20 < y)
					return false;
			}
		}
		return true;
	}
};


class Model
{
public:
	Bird bird;
	vector<Tube*> tubes;
	Rand rand;
	int frame;
	int score;
	bool viewScore;

	Model();
	~Model();

	bool update();
	void flap();
	void reset();
};

#endif // MODEL_H
