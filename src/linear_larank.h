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

#ifndef LINEARLARANK_H_
#define LINEARLARANK_H_

#include "classifier.h"
#include "data.h"
#include "hyperparameters.h"
#include "utilities.h"
#include "linear_larank/LaRank.h"
#include "linear_larank/vectors.h"

class LinearLaRank: public Classifier {
   public:
      /* removed const for minFeatRange and maxFeatRange to allow deserialization */
      /* TODO: is there a way to preserve const? */
      LinearLaRank(const Hyperparameters& hp, const int& numClasses, const int& numFeatures, VectorXd& minFeatRange, VectorXd& maxFeatRange);

      /* default constructor is necessary for serialization. */
      LinearLaRank();

      ~LinearLaRank();

      virtual void update(Sample& sample);
      virtual void eval(Sample& sample, Result& result);

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

   protected:
      Machine* m_svm;
      int m_sampleCount;
      const int* m_numFeatures;

   private:
      /* give access to serialization library */
      friend class boost::serialization::access;
      BOOST_SERIALIZATION_SPLIT_MEMBER();
      template <class Archive>
      void save(Archive& ar, const unsigned int version) const {
         ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Classifier);
         ar & BOOST_SERIALIZATION_NVP(*m_svm);
         ar & BOOST_SERIALIZATION_NVP(m_sampleCount);
         ar & BOOST_SERIALIZATION_NVP(const_cast<int&>(*m_numFeatures));
      }

      template <class Archive>
      void load(Archive& ar, const unsigned int version) {
         ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Classifier);
         ar & BOOST_SERIALIZATION_NVP(*m_svm);
         ar & BOOST_SERIALIZATION_NVP(m_sampleCount);
         ar & BOOST_SERIALIZATION_NVP(const_cast<int&>(*m_numFeatures));
      }
};

#endif /* LINEARLARANK_H_ */
