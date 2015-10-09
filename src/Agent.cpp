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
#include <algorithm>
#include <cmath>

Agent::Agent(Model& model, Json::Value& importJSON)
: 	randNum(0), m_model(model), state(model, importJSON), previousState(""), died(false), flapped(false),
	playing(true), viewMax(false), qFlap(0), qNoFlap(0), 
	rand(0), nn(rand)
{
	// search for import value, or assign second parameter as default
	explorationRate = importJSON["AgentSettings"].get("ExplorationRate", 1000 ).asDouble();
	AgentSettingsJSON["ExplorationRate"] = explorationRate;

	learningRate = importJSON["AgentSettings"].get("LearningRate", 1.0 ).asDouble();
	AgentSettingsJSON["LearningRate"] = learningRate;

	discountFactor = importJSON["AgentSettings"].get("DiscountFactor", 0.99 ).asDouble();
	AgentSettingsJSON["DiscountFactor"] = discountFactor;

	flapRate = importJSON["AgentSettings"].get("FlapRate", 25 ).asDouble();
	AgentSettingsJSON["FlapRate"] = flapRate;

	scaledExplorer = importJSON["AgentSettings"].get("ScaledExplorer", true ).asBool();
	AgentSettingsJSON["ScaledExplorer"] = scaledExplorer;

	#ifdef QTABLE
	AgentSettingsJSON["qTable"] = true;
	#endif

	#ifdef NNET
	AgentSettingsJSON["nnet"] = true;
	nn.m_layers.push_back(new Layer(4, 16));
	nn.m_layers.push_back(new Layer(16, 1));
	nn.init();
	#endif
	
}

Agent::~Agent()
{
}

void Agent::explore()
{
	flap( randNum.next(flapRate) == 0 );
}

bool Agent::exploration()
{
	if(explorationRate == 0)
		return false;

	// increase exploration rate relative to absolute value of maximum Q
	double scaledRate = explorationRate;
	if(scaledExplorer)
		scaledRate *= std::max( std::abs(qFlap), std::abs(qNoFlap) );

	if(scaledRate == 0)
		return true;
	return ( randNum.next(scaledRate) == 0 );
}

void Agent::exploit()
{
	// explore if equal
	if( qFlap == qNoFlap )
		explore();

	// exploit
	flap( qFlap > qNoFlap );
}

void Agent::flap(bool flap)
{
	if (flap)
		m_model.flap();
	flapped = flap;
}

bool Agent::update()
{
	// update qTable
	qt.setQ(previousState + to_str(flapped), calculateQ());

	// save state for next frame
	previousState = state.toCSV();
	
	// agent decision process
	if(playing){
		qFlap 	= qt.getQ( previousState + to_str(true) );
		qNoFlap	= qt.getQ( previousState + to_str(false) );
		if( exploration() )
			explore();
		else 	exploit();
	} else	flap(false);

	return true;
}

double Agent::calculateQ()
{
	/*// combine previous state with last action
	std::string s;
	s = previousState;
	s += to_str(flapped);*/
	
	// current reward
	double 	qvalue = 0.01;
	if(died)
		qvalue = -1;

	// add discounted max next state
	double qFlap 	= qt.getQ( state.toCSV() + to_str(true) );
	double qNoFlap 	= qt.getQ( state.toCSV() + to_str(false) );
	double maxQ = std::max(qFlap, qNoFlap);
	qvalue += (discountFactor * maxQ);

	return qvalue;
}
