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
	qt.readTable("");
	#endif

	#ifdef NNET
	AgentSettingsJSON["nnet"] = true;
	nn.m_layers.push_back(new Layer(5, 16));
	nn.m_layers.push_back(new Layer(16, 1));
	nn.init();

	// get vector from shuffled qTable
	std::vector< std::vector<double> > tabVec;
	std::cout << "writing qTable to vector\n";
	tabVec = qt.toVector();
	std::random_shuffle( tabVec.begin(), tabVec.end() );

	// train nnet
	std::cout << "Training...\n";
	std::vector<double> in;
	in.resize(5);
	std::vector<double> out;
	out.resize(1);
	for ( size_t i = 0; i < tabVec.size(); i++ ){
		// 4 state vars
		in[0] = tabVec[i][0];
		in[1] = tabVec[i][1];
		in[2] = tabVec[i][2];
		in[3] = tabVec[i][3];
		in[4] = tabVec[i][4];
		out[0] = tabVec[i][5];
		//for ( int i = 0; i < 100; i++ ) // calling refine multiple times has no effect on sse
			nn.refine(in, out, 0.02);
	}

	// Test it
	double sse = 0.0;
	int sample = 100;
	if ( tabVec.size() < sample )
		sample = tabVec.size();
	std::cout << "Testing " << sample << " samples...\n";
	for( size_t i = 0; i < sample; i++ )
	{
		in[0] = tabVec[i][0];
		in[1] = tabVec[i][1];
		in[2] = tabVec[i][2];
		in[3] = tabVec[i][3];
		in[4] = tabVec[i][4];
		const vector<double>& prediction = nn.forward_prop(in);
		out[0] = tabVec[i][5];
		//std::cout << "prediction: " << prediction[0] << "\n";
		//std::cout << "actual: " << out[0] << " samples...\n";
		double err0 = out[0] - prediction[0];
		sse += (err0 * err0);
	}
	double rmse = std::sqrt( sse / sample );
	if(rmse < 0.05)
		std::cout << "Passed.\n";
	else
		std::cout << "Failed!!! Got " << to_str(rmse) << "\n";

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
	setQ(previousState + to_str(flapped), calculateQ());

	// save state for next frame
	previousState = state.toCSV();
	
	// agent decision process
	if(playing){
		qFlap 	= getQ( previousState + to_str(true) );
		qNoFlap	= getQ( previousState + to_str(false) );
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
	double qFlap 	= getQ( state.toCSV() + to_str(true) );
	double qNoFlap 	= getQ( state.toCSV() + to_str(false) );
	double maxQ = std::max(qFlap, qNoFlap);
	qvalue += (discountFactor * maxQ);

	return qvalue;
}

double Agent::getQ(std::string stateAction){
	return qt.getQ(stateAction);
}

void Agent::setQ(std::string stateAction, double q){
	qt.setQ(stateAction, q);
}
