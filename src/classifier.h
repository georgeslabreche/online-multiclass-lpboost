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

#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>

#include "data.h"
#include "hyperparameters.h"

using namespace std;

class Classifier {
    public:
        Classifier(const Hyperparameters& hp, const int& numClasses);

        /* default constructor is necessary for serialization. */
        Classifier();
        
        virtual ~Classifier();

        virtual void update(Sample& sample) = 0;
        virtual void eval(Sample& sample, Result& result) = 0;

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

        const string name() const {
            return m_name;
        }

    protected:
        const int* m_numClasses;
        const Hyperparameters* m_hp;
        string m_name;


    private :
    friend class boost::serialization::access;
    BOOST_SERIALIZATION_SPLIT_MEMBER();
    template <class Archive>
    void save(Archive& ar, const unsigned int version) const {
        ar & BOOST_SERIALIZATION_NVP(const_cast<int&>(*m_numClasses));
        ar & BOOST_SERIALIZATION_NVP(const_cast<Hyperparameters&>(*m_hp));
        ar & BOOST_SERIALIZATION_NVP(m_name);
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int version) {
        ar & BOOST_SERIALIZATION_NVP(const_cast<int&>(*m_numClasses));
        ar & BOOST_SERIALIZATION_NVP(const_cast<Hyperparameters&>(*m_hp));
        ar & BOOST_SERIALIZATION_NVP(m_name);
    }
};

#endif /* CLASSIFIER_H_ */
