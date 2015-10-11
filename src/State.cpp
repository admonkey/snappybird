/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler, Jeff Puckett

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#include "State.h"

State::State(Model& model, Json::Value& importJSON)
: m_model(model)
{
	// search for import value, or assign second parameter as default
	discretizer = importJSON["StateSettings"].get("Discretizer", 1.0 ).asDouble();
	StateSettingsJSON["Discretizer"] = discretizer;
	
	// import state variables
	if( importJSON["StateSettings"]["Variables"].isNull() )
		defaultState();
	else	StateSettingsJSON["Variables"] = importJSON["StateSettings"]["Variables"];
}

State::~State()
{
}

void State::defaultState()
{
	ClassicStateVars();
}

void State::ClassicStateVars()
{
	StateSettingsJSON["Variables"]["001"] = "Bird.Velocity-Y";
	StateSettingsJSON["Variables"]["002"] = "Bird-NextTube.Delta-Y";
	StateSettingsJSON["Variables"]["003"] = "Bird-NextTube.Delta-X";
	StateSettingsJSON["Variables"]["004"] = "NextTube.Up";
}

std::string State::toCSV()
{
	//return toDynamicCSV();
	return toClassicStateCSV();
}

std::string State::toDynamicCSV() // FIX: major slow
{
	std::string s = "";
	Json::ValueIterator itr = StateSettingsJSON["Variables"].begin();
	for (  size_t i=0; i<StateSettingsJSON["Variables"].size(); i++ ){
		if ( itr.key()=="001" ){ // Bird.Velocity-Y
			s += to_str(m_model.bird.vert_vel / discretizer) + ",";
			itr++; continue;
		}
		if ( itr.key()=="002" ){ // Bird-NextTube.Delta-Y
			// find next tube
			if(m_model.tubes.size() > 0){
			  int i = 0;
			  while(m_model.tubes[i]->x < m_model.bird.x){
				i++;
			  }
			  // difference between bird-y and tube-y
			  s += to_str((m_model.bird.y - m_model.tubes[i]->y) / discretizer) + ",";
			}
			else s += to_str((m_model.bird.y - 500) / discretizer) + ",";
			itr++; continue;
		}
		if ( itr.key()=="003" ){ // Bird-NextTube.Delta-X
			// find next tube
			if(m_model.tubes.size() > 0){
			  int i = 0;
			  while(m_model.tubes[i]->x < m_model.bird.x)	i++;
			  // difference between bird-x and tube-x
	  		  s += to_str((m_model.tubes[i]->x - m_model.bird.x) / discretizer) + ",";
			}
			else s += to_str(400 / discretizer) + ",";
			itr++; continue;
		}
		if ( itr.key()=="004" ){ // NextTube.Up
			// find next tube
			if(m_model.tubes.size() > 0){
			  int i = 0;
			  while(m_model.tubes[i]->x < m_model.bird.x)	i++;
			  // difference between bird-x and tube-x
	  		  s += to_str(m_model.tubes[i]->up) + ",";
			}
			else s += "1,";
			itr++; continue;
		}
	}
	return s;
}

std::string State::toClassicStateCSV()
{
	std::string s = "";
	// find next tube
	if(m_model.tubes.size() > 0){
	  int i = 0;
	  while(m_model.tubes[i]->x < m_model.bird.x){
		i++;
	  }
	  // next tube up/down
	  s += to_str(m_model.tubes[i]->up);
	  // difference between bird-y and tube-y
	  s += "," + to_str((m_model.bird.y - m_model.tubes[i]->y) / discretizer);
	  // difference between bird-x and tube-x
	  s += "," + to_str((m_model.tubes[i]->x - m_model.bird.x) / discretizer);
	}
	else s += "1," + to_str((m_model.bird.y - 500) / discretizer) + "," + to_str(400 / discretizer);

	// bird-y velocity
	s += "," + to_str(m_model.bird.vert_vel / discretizer) + ",";
	return s;
}

std::vector<double> State::toVector(){
	return toClassicStateVector();
}

std::vector<double> State::toClassicStateVector(){
	std::vector<double> vec;
	// find next tube
	if(m_model.tubes.size() > 0){
	  int i = 0;
	  while(m_model.tubes[i]->x < m_model.bird.x){
		i++;
	  }
	  // next tube up/down
	  vec.push_back( m_model.tubes[i]->up?1.0:0.0 );
	  // difference between bird-y and tube-y
	  vec.push_back( (m_model.bird.y - m_model.tubes[i]->y) / discretizer );
	  // difference between bird-x and tube-x
	  vec.push_back( (m_model.tubes[i]->x - m_model.bird.x) / discretizer );
	}
	else {
	  vec.push_back( 1 );
	  vec.push_back( (m_model.bird.y - 500) / discretizer );
	  vec.push_back( 400 / discretizer );
	}

	// bird-y velocity
	vec.push_back( m_model.bird.vert_vel / discretizer );

	return vec;
}
