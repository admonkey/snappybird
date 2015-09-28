/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler, Jeff Puckett

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#include "Model.h"

#ifndef STATE_H
#define STATE_H

class State
{
public:
	Model& m_model;
	int state;

public:
	State(Model& m);
	virtual ~State();
};

#endif // STATE_H
