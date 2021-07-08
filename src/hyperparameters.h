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

#ifndef HYPERPARAMETERS_H_
#define HYPERPARAMETERS_H_

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>

#include <string>
using namespace std;

typedef enum {
    EXPONENTIAL, LOGIT
} LOSS_FUNCTION;

typedef enum {
    WEAK_ORF, WEAK_LARANK
} WEAK_LEARNER;

// FIXME: Create Hyperparameters subclasses that are specific to the methodology they are tied to.
//        The current approach of using a single monolithic Hyperparameter class for all methodologies
//        results in serialized models that contain unrelated hyperparameter data.
class Hyperparameters {
    public:
        Hyperparameters();
        Hyperparameters(const string& confFile, const string& dataFile, const string& labelsFile);

        virtual void save(const std::string& filename) {
            std::ofstream ofs(filename);
            boost::archive::text_oarchive oa(ofs);
            oa << *this;
            ofs.close();
        }

        virtual void load(const std::string& filename) {
            std::ifstream ifs(filename);
            boost::archive::text_iarchive ia(ifs);
            ia >> *this;
            ifs.close();
        }

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

    private:
        /* give access to serialization library */
        friend class boost::serialization::access;
        BOOST_SERIALIZATION_SPLIT_MEMBER();
        template <class Archive>
        void save(Archive& ar, const unsigned int version) const {
            
            // Forest
            ar & BOOST_SERIALIZATION_NVP(numRandomTests);
            ar & BOOST_SERIALIZATION_NVP(counterThreshold);
            ar & BOOST_SERIALIZATION_NVP(maxDepth);
            ar & BOOST_SERIALIZATION_NVP(numTrees);

            // Linear LaRank
            ar & BOOST_SERIALIZATION_NVP(larankC);

            // Boosting
            ar & BOOST_SERIALIZATION_NVP(numBases);
            ar & BOOST_SERIALIZATION_NVP(weakLearner);

            // Online MCBoost
            ar & BOOST_SERIALIZATION_NVP(shrinkage);
            ar & BOOST_SERIALIZATION_NVP(lossFunction);

            // Online MCLPBoost
            ar & BOOST_SERIALIZATION_NVP(C);
            ar & BOOST_SERIALIZATION_NVP(cacheSize);
            ar & BOOST_SERIALIZATION_NVP(nuD);
            ar & BOOST_SERIALIZATION_NVP(nuP);
            ar & BOOST_SERIALIZATION_NVP(annealingRate);
            ar & BOOST_SERIALIZATION_NVP(theta);
            ar & BOOST_SERIALIZATION_NVP(numIterations);

            // Experimenter
            ar & BOOST_SERIALIZATION_NVP(findTrainError);
            ar & BOOST_SERIALIZATION_NVP(numEpochs);

            // Data
            ar & BOOST_SERIALIZATION_NVP(trainData);
            ar & BOOST_SERIALIZATION_NVP(trainLabels);
            ar & BOOST_SERIALIZATION_NVP(testData);
            ar & BOOST_SERIALIZATION_NVP(testLabels);

            // Output
            ar & BOOST_SERIALIZATION_NVP(savePath);
            ar & BOOST_SERIALIZATION_NVP(verbose);
        }

        template <class Archive>
        void load(Archive& ar, const unsigned int version) {
            
            // Forest
            ar & BOOST_SERIALIZATION_NVP(numRandomTests);
            ar & BOOST_SERIALIZATION_NVP(counterThreshold);
            ar & BOOST_SERIALIZATION_NVP(maxDepth);
            ar & BOOST_SERIALIZATION_NVP(numTrees);

            // Linear LaRank
            ar & BOOST_SERIALIZATION_NVP(larankC);

            // Boosting
            ar & BOOST_SERIALIZATION_NVP(numBases);
            ar & BOOST_SERIALIZATION_NVP(weakLearner);

            // Online MCBoost
            ar & BOOST_SERIALIZATION_NVP(shrinkage);
            ar & BOOST_SERIALIZATION_NVP(lossFunction);

            // Online MCLPBoost
            ar & BOOST_SERIALIZATION_NVP(C);
            ar & BOOST_SERIALIZATION_NVP(cacheSize);
            ar & BOOST_SERIALIZATION_NVP(nuD);
            ar & BOOST_SERIALIZATION_NVP(nuP);
            ar & BOOST_SERIALIZATION_NVP(annealingRate);
            ar & BOOST_SERIALIZATION_NVP(theta);
            ar & BOOST_SERIALIZATION_NVP(numIterations);

            // Experimenter
            ar & BOOST_SERIALIZATION_NVP(findTrainError);
            ar & BOOST_SERIALIZATION_NVP(numEpochs);

            // Data
            ar & BOOST_SERIALIZATION_NVP(trainData);
            ar & BOOST_SERIALIZATION_NVP(trainLabels);
            ar & BOOST_SERIALIZATION_NVP(testData);
            ar & BOOST_SERIALIZATION_NVP(testLabels);

            // Output
            ar & BOOST_SERIALIZATION_NVP(savePath);
            ar & BOOST_SERIALIZATION_NVP(verbose);
        }
};

#endif /* HYPERPARAMETERS_H_ */