/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler, Jeff Puckett

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

#include "State.h"

State::State(Model& model)
: m_model(model), state(0)
{
}

State::~State()
{
}

