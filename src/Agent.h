/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler, Jeff Puckett

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#include "rand.h"
#include "State.h"

#ifndef AGENT_H
#define AGENT_H

class Agent
{
public:
	State state;
	double explorationRate;
	double learningRate;
	double discountFactor;
	Rand randNum;

public:
	Agent();
	virtual ~Agent();
};

#endif // AGENT_H
