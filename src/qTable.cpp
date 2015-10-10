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

void qTable::readTable(std::string instanceID){
  const char* fname = (instanceID + ".qtable").c_str();
  struct stat buf;
    if (stat(fname, &buf) != -1)
    {
        std::cout << "importing qTable...\n";
	std::ifstream file(fname);
	 if (file.is_open())
	 {
	 	std::string line = "";
	 	std::string s = "";
	 	std::string astate = "";
	 	std::string qvalue = "";
	   	while ( std::getline(file, line) )
		{
			s = "";
			astate = "";
			for(int i = 0; i < line.length(); i++){
				if(line[i] != '=')
					s += line[i];
				else {
					astate = s;
					s = "";
				}
				qvalue = s;
			}
		    tab.insert(std::pair<std::string,double>(astate, atof(qvalue.c_str())));
		}
	   	file.close();
	 }
	 std::cout << "qTable imported.\n";
    }
    else std::cout << "no qTable to import. creating new.\n";
}

// returns a vector of vectors holding every state/action-value in qTable
std::vector< std::vector<double> > qTable::toVector(){
	// vector for all state/action-values in qTable
	std::vector< std::vector<double> > t;
	// vector for state/action-value
	std::vector<double> v;
	// string for state variables
	std::string val = "";
	// iterate through qTable
	std::map<std::string,double>::iterator iter;
	for ( iter = tab.begin(); iter != tab.end(); ++iter ) {
		// get state variables
		for ( size_t i = 0; i < iter->first.length(); i++ ) {
			// split vars on comma delimiter
			if ( iter->first[i] != ',' )
				val += iter->first[i];
			else {
				// add state variable to vector
				v.push_back(   atof( val.c_str() )   );
				val = "";
			}
		}
		// add action (last value in string) to vector
		v.push_back(   atof( val.c_str() )   );
		val = "";
		// get q value
		v.push_back( iter->second );
		// add state/action-value to table vector
		t.push_back( v );
	}
	return t;
}
