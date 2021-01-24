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

#include "online_mcboost.h"

/* removed const for minFeatRange and maxFeatRange to allow deserialization */
/* TODO: is there a way to preserve const? */
OnlineMCBoost::OnlineMCBoost(const Hyperparameters& hp, const int& numClasses, const int& numFeatures, 
                             VectorXd& minFeatRange, VectorXd& maxFeatRange) :
    Booster(hp, numClasses, numFeatures, minFeatRange, maxFeatRange) {
    m_name = "OnlineMCBoost";
}

void OnlineMCBoost::update(Sample& sample) {
    sample.w = 1.0;
    double fy = 0.0;
    for (int nBase = 0; nBase < m_hp->numBases; nBase++) {
        Result baseResult(*m_numClasses);
        m_bases[nBase]->update(sample);
        m_bases[nBase]->eval(sample, baseResult);
        fy += m_hp->shrinkage * (baseResult.confidence(sample.y) - 1.0 / (*m_numClasses));

        switch (m_hp->lossFunction) {
        case EXPONENTIAL: {
            sample.w = d_exp(fy);
            break;
        }
        case LOGIT: {
            sample.w = d_logit(fy);
            break;
        }
        }
    }
}
