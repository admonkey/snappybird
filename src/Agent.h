/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler, Jeff Puckett

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#include <iostream>
#include "Model.h"
#include "rand.h"
#include "State.h"
#include "qTable.h"

#ifndef AGENT_H
#define AGENT_H

class Agent
{
public:
	Model& m_model;
	State state;
	double explorationRate;
	double learningRate;
	double discountFactor;
	Rand randNum;
	bool playing;
	qTable qt;

public:
	Agent(Model& m);
	virtual ~Agent();
	bool update();
};

#endif // AGENT_H
