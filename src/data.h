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

#ifndef DATA_H_
#define DATA_H_
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <Eigen/Core>
#include <Eigen/Array>

using namespace std;
using namespace Eigen;

// DATA CLASSES
class Sample {
    public:
        VectorXd x;
        int y;
        double w;
        int id;

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
        /* give access to serialization library */
        friend class boost::serialization::access;
        BOOST_SERIALIZATION_SPLIT_MEMBER();
        template <class Archive>
        void save(Archive& ar, const unsigned int version) const {

            vector<double> x_vector(x.data(), x.data() + x.size());

            ar & BOOST_SERIALIZATION_NVP(x_vector);
            ar & BOOST_SERIALIZATION_NVP(y);
            ar & BOOST_SERIALIZATION_NVP(w);
            ar & BOOST_SERIALIZATION_NVP(id);
        }

        template <class Archive>
        void load(Archive& ar, const unsigned int version) {

            vector<double> x_vector;

            ar & BOOST_SERIALIZATION_NVP(x_vector);
            ar & BOOST_SERIALIZATION_NVP(y);
            ar & BOOST_SERIALIZATION_NVP(w);
            ar & BOOST_SERIALIZATION_NVP(id);

            x = Map<VectorXd>(&x_vector[0], x_vector.size());
        }
};

class DataSet {
 public:
    void findFeatRange();

    void load(const string& x_filename, const string& y_filename);

    vector<Sample> m_samples;
    int m_numSamples;
    int m_numFeatures;
    int m_numClasses;

    VectorXd m_minFeatRange;
    VectorXd m_maxFeatRange;
};

class Result {
 public:
    Result();
    Result(const int& numClasses);

    VectorXd confidence;
    int prediction;
};

class Cache {
public:
    Cache();
    Cache(const Sample& sample, const int& numBases, const int& numClasses);

    Sample cacheSample;
    double margin;
    int yPrime; // Class with closest margin to the sample

    public:
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
        /* give access to serialization library */
        friend class boost::serialization::access;
        BOOST_SERIALIZATION_SPLIT_MEMBER();
        template <class Archive>
        void save(Archive& ar, const unsigned int version) const {
            ar & BOOST_SERIALIZATION_NVP(cacheSample);
            ar & BOOST_SERIALIZATION_NVP(margin);
            ar & BOOST_SERIALIZATION_NVP(yPrime);
        }

        template <class Archive>
        void load(Archive& ar, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(cacheSample);
            ar & BOOST_SERIALIZATION_NVP(margin);
            ar & BOOST_SERIALIZATION_NVP(yPrime);
        }
};

#endif /* DATA_H_ */
