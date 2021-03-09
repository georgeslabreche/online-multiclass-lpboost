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

class OnlineMCLPBoost : public Booster {
   public:
      /* removed const for minFeatRange and maxFeatRange to allow deserialization */
      /* TODO: is there a way to preserve const? */
      OnlineMCLPBoost(const Hyperparameters& hp, const int& numClasses, const int& numFeatures, VectorXd& minFeatRange, VectorXd& maxFeatRange);

      /* default constructor is necessary for serialization. */
      OnlineMCLPBoost();

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
      double m_nuD;
      double m_nuP;

      /* give access to serialization library */
      friend class boost::serialization::access;
      BOOST_SERIALIZATION_SPLIT_MEMBER();
      template <class Archive>
      void save(Archive& ar, const unsigned int version) const {
         ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Booster);
         ar & BOOST_SERIALIZATION_NVP(m_nuD);
         ar & BOOST_SERIALIZATION_NVP(m_nuP);
      }

      template <class Archive>
      void load(Archive& ar, const unsigned int version) {
         ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Booster);
         ar & BOOST_SERIALIZATION_NVP(m_nuD);
         ar & BOOST_SERIALIZATION_NVP(m_nuP);
      }

   int findYPrime(const Sample& sample, const Result& result);
};

#endif // ONLINE_MCLPBOOST_H
