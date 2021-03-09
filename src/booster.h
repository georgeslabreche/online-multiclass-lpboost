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

#ifndef BOOSTER_H
#define BOOSTER_H

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>

#include "classifier.h"

class Booster : public Classifier {
   public:
      /* removed const for minFeatRange and maxFeatRange to allow deserialization */
      /* TODO: is there a way to preserve const? */
      Booster(const Hyperparameters& hp, const int& numClasses, const int& numFeatures,
            VectorXd& minFeatRange, VectorXd& maxFeatRange);

      /* default constructor is necessary for serialization. */
      Booster();

      ~Booster();
      
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
      vector<Classifier*> m_bases;
      VectorXd m_w;
      vector<Cache> m_cache;

   private:
      /* give access to serialization library */
      friend class boost::serialization::access;
      BOOST_SERIALIZATION_SPLIT_MEMBER();
      template <class Archive>
      void save(Archive& ar, const unsigned int version) const {

         vector<double> m_w_vector(m_w.data(), m_w.data() + m_w.size());

         ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Classifier);
         ar & BOOST_SERIALIZATION_NVP(m_bases);
         ar & BOOST_SERIALIZATION_NVP(m_w_vector);
         ar & BOOST_SERIALIZATION_NVP(m_cache);
      }

      template <class Archive>
      void load(Archive& ar, const unsigned int version) {

         vector<double> m_w_vector;
         
         ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Classifier);
         ar & BOOST_SERIALIZATION_NVP(m_bases);
         ar & BOOST_SERIALIZATION_NVP(m_w_vector);
         ar & BOOST_SERIALIZATION_NVP(m_cache);

         m_w = Map<VectorXd>(&m_w_vector[0], m_w_vector.size());
      }
};

#endif // BOOSTER_H
