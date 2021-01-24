// -*- C++ -*-
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2010 Amir Saffari, amir@ymer.org
 * Copyright (C) 2010 Amir Saffari, 
 *                    Institute for Computer Graphics and Vision, 
 *                    Graz University of Technology, Austria
 * 
 * Modified 2021 Georges Labreche, georges@tanagraspace.org
 * For the OrbitAI experiment onboard ESA's OPS-SAT spacecraft.
 */

#ifndef ONLINE_MCLPBOOST_H
#define ONLINE_MCLPBOOST_H

#include "classifier.h"
#include "booster.h"
#include "data.h"
#include "hyperparameters.h"
#include "utilities.h"

class OnlineMCLPBoost : public Booster {
   public:
      /* removed const for minFeatRange and maxFeatRange to allow deserialization */
      /* TODO: is there a way to preserve const? */
      OnlineMCLPBoost(const Hyperparameters& hp, const int& numClasses, const int& numFeatures, VectorXd& minFeatRange, VectorXd& maxFeatRange);

      virtual void update(Sample& sample);

   private:
      double m_nuD;
      double m_nuP;

   int findYPrime(const Sample& sample, const Result& result);
};

#endif // ONLINE_MCLPBOOST_H
