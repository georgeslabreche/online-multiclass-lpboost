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

#ifndef ONLINERF_H_
#define ONLINERF_H_

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include "classifier.h"
#include "data.h"
#include "hyperparameters.h"
#include "utilities.h"

class RandomTest {
   public:
      /* removed const for minFeatRange and maxFeatRange to allow deserialization */
      /* TODO: is there a way to preserve const? */
      RandomTest(const int& numClasses, const int& numFeatures, VectorXd &minFeatRange, VectorXd &maxFeatRange);

      /* default constructor is necessary for serialization. */
      RandomTest();

      void update(const Sample& sample);
      bool eval(const Sample& sample) const;

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

      double score() const;

      pair<VectorXd, VectorXd > getStats() const;
   
   protected:
      const int* m_numClasses;
      int m_feature;
      double m_threshold;

      double m_trueCount;
      double m_falseCount;
      VectorXd m_trueStats;
      VectorXd m_falseStats;

   void updateStats(const Sample& sample, const bool& decision);


   private:
      /* give access to serialization library */
      friend class boost::serialization::access;
      BOOST_SERIALIZATION_SPLIT_MEMBER();
      template <class Archive>
      void save(Archive& ar, const unsigned int version) const {
         vector<double> trueStats_vector(m_trueStats.data(), m_trueStats.data() + m_trueStats.size());
         vector<double> falseStats_vector(m_falseStats.data(), m_falseStats.data() + m_falseStats.size());

         
         ar & BOOST_SERIALIZATION_NVP(const_cast<int&>(*m_numClasses));
         ar & BOOST_SERIALIZATION_NVP(m_feature);
         ar & BOOST_SERIALIZATION_NVP(m_threshold);
         ar & BOOST_SERIALIZATION_NVP(m_trueCount);
         ar & BOOST_SERIALIZATION_NVP(m_falseCount);
         ar & BOOST_SERIALIZATION_NVP(trueStats_vector);
         ar & BOOST_SERIALIZATION_NVP(falseStats_vector);
      }

      template <class Archive>
      void load(Archive& ar, const unsigned int version) {

         vector<double> trueStats_vector;
         vector<double> falseStats_vector;

         ar & BOOST_SERIALIZATION_NVP(const_cast<int&>(*m_numClasses));
         ar & BOOST_SERIALIZATION_NVP(m_feature);
         ar & BOOST_SERIALIZATION_NVP(m_threshold);
         ar & BOOST_SERIALIZATION_NVP(m_trueCount);
         ar & BOOST_SERIALIZATION_NVP(m_falseCount);
         ar & BOOST_SERIALIZATION_NVP(trueStats_vector);
         ar & BOOST_SERIALIZATION_NVP(falseStats_vector);

         m_trueStats = Map<VectorXd>(&trueStats_vector[0], trueStats_vector.size());
         m_falseStats = Map<VectorXd>(&falseStats_vector[0], falseStats_vector.size());
      }
};

class OnlineNode {
   public:

      /* removed const for minFeatRange and maxFeatRange to allow deserialization */
      /* TODO: is there a way to preserve const? */
      OnlineNode(const Hyperparameters& hp, const int& numClasses, const int& numFeatures, VectorXd& minFeatRange, VectorXd& maxFeatRange, 
            const int& depth);
      OnlineNode(const Hyperparameters& hp, const int& numClasses, const int& numFeatures, VectorXd& minFeatRange, VectorXd& maxFeatRange, 
            const int& depth, const VectorXd& parentStats);

      /* default constructor is necessary for serialization. */
      OnlineNode();
      
      ~OnlineNode();
      
      void update(const Sample& sample);
      void eval(const Sample& sample, Result& result);

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
      const int* m_numClasses;
      int m_depth;
      bool m_isLeaf;
      const Hyperparameters* m_hp;
      int m_label;
      double m_counter;
      double m_parentCounter;
      VectorXd m_labelStats;

      /* removed const to allow deserialization */
      /* TODO: is there a way to preserve const? */
      VectorXd* m_minFeatRange;
      VectorXd* m_maxFeatRange;
      
      OnlineNode* m_leftChildNode;
      OnlineNode* m_rightChildNode;
      
      vector<RandomTest*> m_onlineTests;
      RandomTest* m_bestTest;
      
      bool shouldISplit() const;


      /* give access to serialization library */
      friend class boost::serialization::access;
      BOOST_SERIALIZATION_SPLIT_MEMBER();
      template <class Archive>
      void save(Archive& ar, const unsigned int version) const {

         vector<double> labelStats_vector(m_labelStats.data(), m_labelStats.data() + m_labelStats.size());
         vector<double> minFeatRange_vector(m_minFeatRange->data(), m_minFeatRange->data() + m_minFeatRange->size());
         vector<double> maxFeatRange_vector(m_maxFeatRange->data(), m_maxFeatRange->data() + m_maxFeatRange->size());
         
         ar & BOOST_SERIALIZATION_NVP(const_cast<int&>(*m_numClasses));
         ar & BOOST_SERIALIZATION_NVP(m_depth);
         ar & BOOST_SERIALIZATION_NVP(m_isLeaf);
         ar & BOOST_SERIALIZATION_NVP(const_cast<Hyperparameters&>(*m_hp));
         ar & BOOST_SERIALIZATION_NVP(m_label);
         ar & BOOST_SERIALIZATION_NVP(m_counter);
         ar & BOOST_SERIALIZATION_NVP(m_parentCounter);

         /** FIXME: Segmentation fault (core dumped)
         ar & BOOST_SERIALIZATION_NVP(*m_leftChildNode);
         ar & BOOST_SERIALIZATION_NVP(*m_rightChildNode);
         */

         /** FIXME: triggers a pointer conflict error
          * http://www.bnikolic.co.uk/blog/cpp-boost-ser-conflict.html 
          * 
          * Maybe use smart pointers?
          */
         //ar & BOOST_SERIALIZATION_NVP(m_onlineTests);

         ar & BOOST_SERIALIZATION_NVP(*m_bestTest);
         ar & BOOST_SERIALIZATION_NVP(labelStats_vector);
         ar & BOOST_SERIALIZATION_NVP(minFeatRange_vector);
         ar & BOOST_SERIALIZATION_NVP(maxFeatRange_vector);
         
      }

      template <class Archive>
      void load(Archive& ar, const unsigned int version) {
         vector<double> labelStats_vector;
         vector<double> minFeatRange_vector;
         vector<double> maxFeatRange_vector;

         ar & BOOST_SERIALIZATION_NVP(const_cast<int&>(*m_numClasses));
         ar & BOOST_SERIALIZATION_NVP(m_depth);
         ar & BOOST_SERIALIZATION_NVP(m_isLeaf);
         ar & BOOST_SERIALIZATION_NVP(const_cast<Hyperparameters&>(*m_hp));

         
         ar & BOOST_SERIALIZATION_NVP(m_label);
         ar & BOOST_SERIALIZATION_NVP(m_counter);
         ar & BOOST_SERIALIZATION_NVP(m_parentCounter);

         /**
         ar & BOOST_SERIALIZATION_NVP(*m_leftChildNode);
         ar & BOOST_SERIALIZATION_NVP(*m_rightChildNode);
         **/

         //ar & BOOST_SERIALIZATION_NVP(m_onlineTests);
         ar & BOOST_SERIALIZATION_NVP(*m_bestTest);

         ar & BOOST_SERIALIZATION_NVP(labelStats_vector);
         ar & BOOST_SERIALIZATION_NVP(minFeatRange_vector);
         ar & BOOST_SERIALIZATION_NVP(maxFeatRange_vector);

         m_labelStats = Map<VectorXd>(&labelStats_vector[0], labelStats_vector.size());
         *m_minFeatRange = Map<VectorXd>(&minFeatRange_vector[0], minFeatRange_vector.size());
         *m_maxFeatRange = Map<VectorXd>(&maxFeatRange_vector[0], maxFeatRange_vector.size());
      }
};


class OnlineTree: public Classifier {
   public:

      /* removed const for minFeatRange and maxFeatRange to allow deserialization */
      /* TODO: is there a way to preserve const? */
      OnlineTree(const Hyperparameters& hp, const int& numClasses, const int& numFeatures, VectorXd& minFeatRange, VectorXd& maxFeatRange);
      
      /* default constructor is necessary for serialization. */
      OnlineTree();

      ~OnlineTree();

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
   
   private:
      OnlineNode* m_rootNode;


      /* give access to serialization library */
      friend class boost::serialization::access;
      BOOST_SERIALIZATION_SPLIT_MEMBER();
      template <class Archive>
      void save(Archive& ar, const unsigned int version) const {
         ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Classifier);
         ar & BOOST_SERIALIZATION_NVP(*m_rootNode);
      }

      template <class Archive>
      void load(Archive& ar, const unsigned int version) {
         ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Classifier);
         ar & BOOST_SERIALIZATION_NVP(*m_rootNode);
      }
};


class OnlineRF: public Classifier {
 public:

   /* removed const for minFeatRange and maxFeatRange to allow deserialization */
   /* TODO: is there a way to preserve const? */
   OnlineRF(const Hyperparameters& hp, const int& numClasses, const int& numFeatures, VectorXd& minFeatRange, VectorXd& maxFeatRange);

   ~OnlineRF();
    
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
      double m_counter;
      double m_oobe;
      vector<OnlineTree*> m_trees;

   private:
      /* give access to serialization library */
      friend class boost::serialization::access;
      BOOST_SERIALIZATION_SPLIT_MEMBER();
      template <class Archive>
      void save(Archive& ar, const unsigned int version) const {
         ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Classifier);
         ar & BOOST_SERIALIZATION_NVP(m_counter);
         ar & BOOST_SERIALIZATION_NVP(m_oobe);

         /** 
          * For vector of poitners we can just do "ar & m_trees" based on this:
          * https://www.boost.org/doc/libs/1_55_0/libs/serialization/doc/tutorial.html#pointers
          */
         ar & BOOST_SERIALIZATION_NVP(m_trees);
      }

      template <class Archive>
      void load(Archive& ar, const unsigned int version) {
         ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Classifier);
         ar & BOOST_SERIALIZATION_NVP(m_counter);
         ar & BOOST_SERIALIZATION_NVP(m_oobe);
         ar & BOOST_SERIALIZATION_NVP(m_trees);
      }
};

#endif /* ONLINERF_H_ */
