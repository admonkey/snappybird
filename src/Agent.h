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
#include "neuralnet.h"

#ifndef AGENT_H
#define AGENT_H

class Agent
{
public:
	Json::Value AgentSettingsJSON;
	double explorationRate;
	double learningRate;
	double discountFactor;
	double flapRate;
	bool scaledExplorer;

	Rand randNum;
	Model& m_model;
	State state;
	std::string previousState;
	bool died;
	bool flapped;
	#define QTABLE
	qTable qt;
	
	#define NNET
	Rand rand;
	NeuralNet nn;

	bool playing;
	bool viewMax;
	double qFlap;
	double qNoFlap;

public:
	Agent(Model& m, Json::Value& importJSON);
	virtual ~Agent();
	bool update();
	double calculateQ();
	void explore();
	void exploit();
	bool exploration();
	void flap(bool flap);
	double getQ(std::string stateAction);
	void setQ(std::string stateAction, double q);
};

#endif // AGENT_H
