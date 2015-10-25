/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler, Jeff Puckett

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#include <string>
#include <map>
#include <iostream>
#include <sys/stat.h>
#include <vector>

#ifndef QTABLE_H
#define QTABLE_H

class qTable
{
public:
	std::map<std::string,double> tab;
	bool viewQ;

public:
	qTable();
	virtual ~qTable();
	double getQ(std::string stateAction);
	void setQ(std::string stateAction, double qVal);
	void writeTable(std::string instanceID);
	void readTable(std::string instanceID);
	std::vector< std::vector<double> > toVector();
};

#endif // QTABLE_H