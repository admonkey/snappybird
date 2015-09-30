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

Agent::Agent(Model& model, Json::Value& importJSON)
: 	explorationRate(1000), learningRate(1.0), discountFactor(0.99), flapRate(25),
	randNum(0), m_model(model), state(model, importJSON), previousState(""), died(false), flapped(false),
	playing(true), viewMax(false)
{
	//if(importJSON.isNull())
		//std::cout << "import null\n";
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
	return ( randNum.next(explorationRate) == 0 );
}

bool Agent::exploit()
{
	std::string currentState = state.toCSV();
	double qFlap 	= qt.getQ( currentState + to_str(true) );
	double qNoFlap 	= qt.getQ( currentState + to_str(false) );
	double maxQ = std::max(qFlap, qNoFlap);

	//if(viewMax && (qFlap != 0)) {
	if(viewMax) {
		std::replace( currentState.begin(), currentState.end(), ',', '\t');
		std::cout << "\t\t\t" << currentState << to_str(true) << "\tgetQ: " << qFlap << "\n";
		std::cout << "\t\t\t" << currentState << to_str(false) << "\tgetQ: " << qNoFlap << "\t\tmax:" << maxQ << "\n";
		//std::cout << "\t\texploiting: qFlap" << to_str(true) << ": " << qFlap << "\n";
		//std::cout << "\t\texploiting: qNoFlap" << to_str(false) << ": " << qNoFlap << " max:" << maxQ << "\n";
	}
	
	// avoid punishment
	if( qNoFlap < 0 ){
		flap( true );
		return true;
	}
	if( qFlap < 0 ){
		flap( false );
		return true;
	}

	// exploratory conditions
	if( maxQ == 0 )
		return false;
	if( qFlap == qNoFlap )
		return false;
	if( exploration() )
		return false;

	//if( qFlap > qNoFlap ) std::cout << "flap\n";
	flap( qFlap > qNoFlap );
	return true;
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
	//std::cout << "getQ: " << qt.getQ(previousState + to_str(flapped)) << "\n";

	// save state for next frame
	previousState = state.toCSV();
	
	// agent decision process
	if(playing){
		if(!exploit()) explore();
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
		return qvalue = -0.01;

	// add discounted max next state
	double qFlap 	= qt.getQ( state.toCSV() + to_str(true) );
	double qNoFlap 	= qt.getQ( state.toCSV() + to_str(false) );
	double maxQ = std::max(qFlap, qNoFlap);
	qvalue += (discountFactor * maxQ);

	return qvalue;
}
