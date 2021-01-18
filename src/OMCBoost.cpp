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

//#if 0
#include <cstdlib>
#include <iostream>
#include <string>
#include <string.h>
#include <libconfig.h++>

#include "data.h"
#include "experimenter.h"
#include "online_rf.h"
#include "linear_larank.h"
#include "online_mcboost.h"
#include "online_mclpboost.h"

using namespace std;
using namespace libconfig;

// Flag to toggle training testability.
#define TESTABLE_MODEL 0

typedef enum {
    ORT, ORF, OMCBOOST, OMCLPBOOST, LARANK
} CLASSIFIER_TYPE;

//! Prints the interface help message
void help() {
    cout << endl;
    cout << "OMCBoost Classification Package:" << endl;
    cout << "Input arguments:" << endl;
    cout << "\t -h | --help : \t will display this message." << endl;
    cout << "\t -c : \t\t path to the config file." << endl << endl;
    cout << "\t --data : \t path to the training data file." << endl;
    cout << "\t --labels : \t path to the training labels file." << endl;
    cout << "\t --ort : \t use Online Random Tree (ORT) algorithm." << endl;
    cout << "\t --orf : \t use Online Random Forest (ORF) algorithm." << endl;
    cout << "\t --omcb : \t use Online MCBoost algorithm." << endl;
    cout << "\t --omclp : \t use Online MCLPBoost algorithm." << endl;
    cout << "\t --omclppd : \t use Online MCLPBoostPD algorithm." << endl;
    cout << "\t --larank : \t use Online LaRank algorithm." << endl;
#if TESTABLE_MODEL
    cout << endl << endl;
    cout << "\t --train : \t train the classifier." << endl;
    cout << "\t --test : \t test the classifier." << endl;
    cout << "\t --t2 : \t train and test the classifier at the same time." << endl;
    cout << endl << endl;
    cout << "\tExamples:" << endl;
    cout << "\t ./OMCBoost -c conf/orf.conf --orf --train --test" << endl;
#endif
}

int main(int argc, char *argv[]) {
    // Parsing command line
    string confFileName;
    string dataFilename;
    string labelsFilename;
    string modelFilename;

    int classifier = -1;

#if TESTABLE_MODEL
    int doTraining = false;
#else
    int doTraining = true;
#endif
    int doTesting = false;
    int doT2 = false;

    int inputCounter = 1;

    // Is it a new model or are we updating a previously saved model.
    int updateModel = false;

    if (argc == 1) {
        cout << "\tNo input argument specified: aborting." << endl;
        help();
        exit(EXIT_SUCCESS);
    }

    while (inputCounter < argc) {
        if (!strcmp(argv[inputCounter], "-h") || !strcmp(argv[inputCounter], "--help")) 
        {
            help();
            return EXIT_SUCCESS;
        }
        else if (!strcmp(argv[inputCounter], "-c"))
        {
            confFileName = argv[++inputCounter];
        }
        else if (!strcmp(argv[inputCounter], "--data"))
        {
            dataFilename = argv[++inputCounter];
        }
        else if (!strcmp(argv[inputCounter], "--labels"))
        {
            labelsFilename = argv[++inputCounter];
        }
        else if (!strcmp(argv[inputCounter], "--new") || !strcmp(argv[inputCounter], "--update")) {

            if(!strcmp(argv[inputCounter], "--new"))
            {
                updateModel = false;
            }
            else
            {
                updateModel = true;
            }

            modelFilename = argv[++inputCounter];
        }
        else if (!strcmp(argv[inputCounter], "--ort")) {
            classifier = ORT;
        }
        else if (!strcmp(argv[inputCounter], "--orf")) {
            classifier = ORF;
        }
        else if (!strcmp(argv[inputCounter], "--omcb")) {
            classifier = OMCBOOST;
        }
        else if (!strcmp(argv[inputCounter], "--omclp")) {
            classifier = OMCLPBOOST;
        }
        else if (!strcmp(argv[inputCounter], "--larank"))
        {
            classifier = LARANK;
        }
#if TESTABLE_MODEL 
        else if (!strcmp(argv[inputCounter], "--train")) 
        {
            doTraining = true;
        } 
        else if (!strcmp(argv[inputCounter], "--test"))
        {
            doTesting = true;
        } 
        else if (!strcmp(argv[inputCounter], "--t2"))
        {
            doT2 = true;
        } 
#endif
        else 
        {
            cout << "\tUnknown input argument: " << argv[inputCounter];
            cout << ", please try --help for more information." << endl;
            exit(EXIT_FAILURE);
        }

        inputCounter++;
    }

    cout << "OnlineMCBoost Classification Package:" << endl;

    if (!doTraining && !doTesting && !doT2) {
        cout << "\tNothing to do." << endl;
        exit(EXIT_FAILURE);
    }

#if TESTABLE_MODEL
    if (doT2) {
        doTraining = false;
        doTesting = false;
    }
#endif

    // Load the hyperparameters
    Hyperparameters hp(confFileName, dataFilename, labelsFilename);

    // Creating the train data
    DataSet dataset_tr, dataset_ts;
    dataset_tr.load(hp.trainData, hp.trainLabels);

#if TESTABLE_MODEL
    if (doT2 || doTesting) {
        dataset_ts.load(hp.testData, hp.testLabels);
    }
#endif

    // Calling training/testing
    Classifier* pModel = NULL;

    switch (classifier) {
        case ORT: {
            pModel = new OnlineTree(hp, dataset_tr.m_numClasses, dataset_tr.m_numFeatures,
                                dataset_tr.m_minFeatRange, dataset_tr.m_maxFeatRange);
            break;
        }
        case ORF: {
            pModel = new OnlineRF(hp, dataset_tr.m_numClasses, dataset_tr.m_numFeatures,
                                dataset_tr.m_minFeatRange, dataset_tr.m_maxFeatRange);
            break;
        }
        case OMCBOOST: {
            pModel = new OnlineMCBoost(hp, dataset_tr.m_numClasses, dataset_tr.m_numFeatures,
                                    dataset_tr.m_minFeatRange, dataset_tr.m_maxFeatRange);
            break;
        }
        case OMCLPBOOST: {
            pModel = new OnlineMCLPBoost(hp, dataset_tr.m_numClasses, dataset_tr.m_numFeatures,
                                        dataset_tr.m_minFeatRange, dataset_tr.m_maxFeatRange);
            break;
        }
        case LARANK: {
            pModel = new LinearLaRank(hp, dataset_tr.m_numClasses, dataset_tr.m_numFeatures,
                                    dataset_tr.m_minFeatRange, dataset_tr.m_maxFeatRange);
            break;
        }
    }

    if (pModel) {
#if TESTABLE_MODEL
        if (doT2) {
            trainAndTest(pModel, dataset_tr, dataset_ts, hp);
        }
#endif
        if (doTraining) {
            train(pModel, dataset_tr, hp);
        }
#if TESTABLE_MODEL
        if (doTesting) {
            test(pModel, dataset_ts, hp);
        }
#endif
    }

    // Preparing for exit
    delete pModel;

    return EXIT_SUCCESS;
}
//#endif