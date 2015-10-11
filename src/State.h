/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler, Jeff Puckett

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#include "string.h"
#include "Model.h"
#include "json/json.h"
#include <fstream>
#include <iostream>

#ifndef STATE_H
#define STATE_H

class State
{
public:
	Model& m_model;
	double discretizer;
	Json::Value StateSettingsJSON;

public:
	State(Model& m, Json::Value& importJSON);
	virtual ~State();
	std::string toCSV();
	std::string toDynamicCSV(); // untested
	std::string toClassicStateCSV();
	void defaultState();
	void ClassicStateVars();
	std::vector<double> toVector();
	std::vector<double> toClassicStateVector();
};

#endif // STATE_H
