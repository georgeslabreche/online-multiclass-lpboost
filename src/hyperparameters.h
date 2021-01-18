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
 */

#ifndef HYPERPARAMETERS_H_
#define HYPERPARAMETERS_H_

#include <string>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace std;

typedef enum {
    EXPONENTIAL, LOGIT
} LOSS_FUNCTION;

typedef enum {
    WEAK_ORF, WEAK_LARANK
} WEAK_LEARNER;

class Hyperparameters {
 public:
    Hyperparameters();
    Hyperparameters(const string& confFile, const string& dataFile, const string& labelsFile);

    // Forest
    int numRandomTests;
    int counterThreshold;
    int maxDepth;
    int numTrees;

    // Linear LaRank
    double larankC;

    // Boosting
    int numBases;
    WEAK_LEARNER weakLearner;

    // Online MCBoost
    double shrinkage;
    LOSS_FUNCTION lossFunction;

    // Online MCLPBoost
    double C;
    int cacheSize;
    double nuD;
    double nuP;
    double annealingRate;
    double theta;
    int numIterations;

    // Experimenter
    int findTrainError;
    int numEpochs;

    // Data
    string trainData;
    string trainLabels;
    string testData;
    string testLabels;

    // Output
    string savePath;
    int verbose;
};


namespace boost {
    namespace serialization {

        template<class Archive>
        void serialize(Archive& ar, Hyperparameters& hp, const unsigned int version)
        {
            // Forest
            ar & hp.numRandomTests;
            ar & hp.counterThreshold;
            ar & hp.maxDepth;
            ar & hp.numTrees;

            // Linear LaRank
            ar & hp.larankC;

            // Boosting
            ar & hp.numBases;
            ar & hp.weakLearner;

            // Online MCBoost
            ar & hp.shrinkage;
            ar & hp.lossFunction;

            // Online MCLPBoost
            ar & hp.C;
            ar & hp.cacheSize;
            ar & hp.nuD;
            ar & hp.nuP;
            ar & hp.annealingRate;
            ar & hp.theta;
            ar & hp.numIterations;

            // Experimenter
            ar & hp.findTrainError;
            ar & hp.numEpochs;

            // Data
            ar & hp.trainData;
            ar & hp.trainLabels;
            ar & hp.testData;
            ar & hp.testLabels;

            // Output
            ar & hp.savePath;
            ar & hp.verbose;

        } 
   } // namespace serialization
} // namespace boost

#endif /* HYPERPARAMETERS_H_ */
