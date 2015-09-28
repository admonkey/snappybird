/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler, Jeff Puckett

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#include "State.h"

State::State(Model& model)
: m_model(model), discretizer(1.0)
{
}

State::~State()
{
}

std::string State::getState()
{
	std::string s = "";
	// find next tube
	if(m_model.tubes.size() > 0){
	  int i = 0;
	  while(m_model.tubes[i]->x < m_model.bird.x){
		i++;
	  }
	  // next tube up/down
	  s += to_str(m_model.tubes[i]->up);
	  // difference between bird-y and tube-y
	  s += "," + to_str((m_model.bird.y - m_model.tubes[i]->y) / discretizer);
	  // difference between bird-x and tube-x
	  s += "," + to_str((m_model.tubes[i]->x - m_model.bird.x) / discretizer);
	}
	else s += "0,0," + to_str(400 / discretizer);

	// bird-y velocity
	s += "," + to_str(m_model.bird.vert_vel / discretizer);
	return s;
}

void State::setState()
{
}
