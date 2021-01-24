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

#include "classifier.h"

Classifier::Classifier(const Hyperparameters& hp, const int& numClasses) : 
    m_numClasses(&numClasses), m_hp(&hp) {
}

/* default constructor is necessary for serialization. */
Classifier::Classifier(){
}

Classifier::~Classifier() {
}
