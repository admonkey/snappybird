/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler, Jeff Puckett

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#include "qTable.h"
#include <algorithm>
#include "string.h"
#include <fstream>

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
		std::cout << "\tstateAction:\t" << stateAction << "\tsetQ: " << qVal << "\n";
		//std::cout << "getQ: " << getQ(stateAction) << "\n";
	}
}

void qTable::writeTable(std::string instanceID)
{
	std::cout << "writing qTable to file...\n";
	std::ofstream qfile;
	std::string fname = instanceID + ".qtable";
	qfile.open(fname.c_str(), std::ofstream::out);
	std::map<std::string,double>::iterator iter;
	std::string strToReturn = ""; //This is no longer on the heap
	for (iter = tab.begin(); iter != tab.end(); ++iter) {
	   strToReturn += iter->first; //Not a method call
	   strToReturn += "=";
	   strToReturn += to_str(iter->second);
	   strToReturn += "\n";
	   // Make sure you don't modify table here or the iterators will not work as you expect
	}
	qfile << strToReturn;
	qfile.close();
	
	/*// annonymous double write for default next import
	std::ifstream  src(fname.c_str(), std::ios::binary);
	std::ofstream  dst("qtable",   std::ios::binary);
	dst << src.rdbuf();*/
	
	std::cout << "qTable written to file.\n";
}
