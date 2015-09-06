/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler,

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#include "Model.h"
#include "error.h"
#include "string.h"
#include <stdlib.h>
#include <iostream>

using std::cout;


Model::Model() : rand(0), frame(38), score(0)
{
}

Model::~Model()
{
	for(size_t i = 0; i < tubes.size(); i++)
		delete(tubes[i]);
}

bool Model::update()
{
	if(!bird.update())
		return false;
	for(size_t i = 0; i < tubes.size(); i++) {
		Tube* t = tubes[i];
		if(!t->update(bird))
			return false;
		if(t->x < -55) {
			tubes[i] = tubes[tubes.size() - 1];
			tubes.erase(tubes.begin() + tubes.size() - 1);
			score++;
			//cout << to_str(++score) << "\n";
		}
	}
	if(++frame % 35 == 0) {
		bool up = (rand.next(2) == 0);
		Tube* t = new Tube(rand.next(350) + (up ? 150 : 0), up);
		tubes.push_back(t);
	}
	return true;
}

void Model::flap()
{
	bird.flap();
}
