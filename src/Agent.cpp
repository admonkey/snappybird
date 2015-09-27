/*
  The contents of this file are dedicated by all of its authors, including

    Michael S. Gashler, Jeff Puckett

  to the public domain (http://creativecommons.org/publicdomain/zero/1.0/).
*/

/* 
  Reinforcement Learning Algorithm:
  Q(i, a) ← (1 − L^k)Q(i, a) + L^k [ r(i, a, j) + (γ * max(b∈A(j)) * Q(j, b)) ]
  Q = Quantity of value of action given state
  L^k = learning rate
  i,j = state
  a,b = action
  A = set of possible actions a
  r = reward
  γ = discount factor 
  	note: γ should decay at rate of e^(−µt(i,a,j))
*/ 

#include "Agent.h"

Agent::Agent()
: explorationRate(0.01), learningRate(1.0), discountFactor(0.99), randNum(0)
{
}

Agent::~Agent()
{
}

