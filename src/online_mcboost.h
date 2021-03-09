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

#ifndef ONLINEMCBOOST_H_
#define ONLINEMCBOOST_H_

#include <cmath>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>

#include "classifier.h"
#include "booster.h"
#include "data.h"
#include "hyperparameters.h"
#include "utilities.h"


/* removed const for minFeatRange and maxFeatRange to allow deserialization */
/* TODO: is there a way to preserve const? */
class OnlineMCBoost: public Booster {
    public:
        OnlineMCBoost(const Hyperparameters& hp, const int& numClasses, const int& numFeatures, VectorXd& minFeatRange, VectorXd& maxFeatRange);

        /* default constructor is necessary for serialization. */
        OnlineMCBoost();

        virtual void update(Sample& sample);

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

    private:
        inline double d_exp(const double& fy) {
            return exp(-fy);
        }

        inline double d_logit(const double& fy) {
            return 1.0 / (1.0 + exp(fy));
        }

        /* give access to serialization library */
        friend class boost::serialization::access;
        BOOST_SERIALIZATION_SPLIT_MEMBER();
        template <class Archive>
        void save(Archive& ar, const unsigned int version) const {
            ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Booster);
        }

        template <class Archive>
        void load(Archive& ar, const unsigned int version) {
            ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Booster);
        }
};

#endif /* ONLINEMCBOOST_H_ */
