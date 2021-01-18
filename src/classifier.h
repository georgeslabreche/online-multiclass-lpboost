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

#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include "data.h"
#include "hyperparameters.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace std;

class Classifier {
 public:
    Classifier(const Hyperparameters& hp, const int& numClasses);
    
    virtual ~Classifier();

    virtual void update(Sample& sample) = 0;
    virtual void eval(Sample& sample, Result& result) = 0;

    const string name() const {
        return m_name;
    }

 // Make public or else cannot serialize.
 //protected:
    const int* m_numClasses;
    const Hyperparameters* m_hp;
    string m_name;
};

namespace boost {
    namespace serialization {

        template<class Archive>
        void serialize(Archive& ar, Classifier& classifer, const unsigned int version)
        {
            ar & classifer.m_numClasses;
            ar & classifer.m_hp;
            ar & classifer.m_name;
        }

   } // namespace serialization
} // namespace boost


#endif /* CLASSIFIER_H_ */
