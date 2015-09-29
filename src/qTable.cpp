/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler, Jeff Puckett

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#include "qTable.h"
#include <algorithm>

qTable::qTable() : viewQ(false)
{
}

qTable::~qTable()
{
}

double qTable::getQ(std::string stateAction)
{
	// lookup q-value, return 0.0 if none found
	std::map<std::string,double>::iterator it = tab.find(stateAction);
	double qValue = 0.0;
	if(it != tab.end())
		qValue = it->second;
	return qValue;
}

void qTable::setQ(std::string stateAction, double qVal)
{	
	// update qvalue or insert new
	std::map<std::string,double>::iterator it = tab.find(stateAction);
	if(it != tab.end())
		it->second = qVal;
	else
		tab.insert(std::pair<std::string,double>(stateAction, qVal));
		
	// press Q to view
	if(viewQ) {
		std::replace( stateAction.begin(), stateAction.end(), ',', '\t');
		std::cout << "\t stateAction: " << stateAction << "\tq: " << qVal << "\n";
	}
}
