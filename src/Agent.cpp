/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler, Jeff Puckett

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

/* 
  Reinforcement Learning Algorithm:
  Q(i, a) ← (1 − L^k)Q(i, a) + L^k [ r(i, a, j) + ( γ * max( b∈A(j) ) * Q(j, b) ) ]
  Q = Quantity of value of action given state
  L^k = learning rate
  i,j = state
  a,b = action
  A = set of possible actions a
  r = reward
  γ = discount factor 
  	note: γ should decay at rate of e^( −µt(i,a,j) )
*/ 

#include "Agent.h"

Agent::Agent(Model& model, Json::Value& importJSONc)
: 	explorationRate(0.01), learningRate(1.0), discountFactor(0.99), 
	randNum(0), m_model(model), importJSON(importJSONc), state(model), previousState(""), died(false), flapped(false),
	playing(false)
{
}

Agent::~Agent()
{
}

bool Agent::update()
{
	qt.setQ(previousState + "," + to_str(flapped), calculateQ());
	//std::cout << state.toString() << "\n";
	previousState = state.toCSV();
	return true;
}

double Agent::calculateQ()
{
	// combine previous state with last action
	std::string s;
	s = previousState;
	s += "," + to_str(flapped);
	
	// current reward
	double qvalue = 1.0;
	if(died)
		return qvalue = -1000.0;


	// add discounted max next state
	double qFlap 	= qt.getQ( state.toCSV() + "," + to_str(true) );
	double qNoFlap 	= qt.getQ( state.toCSV() + "," + to_str(false) );
	double maxQ = std::max(qFlap, qNoFlap);
	qvalue += (discountFactor * maxQ);

	return qvalue;
}
