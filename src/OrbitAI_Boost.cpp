#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read

#include <libconfig.h++>

//#include "data.h"
//#include "experimenter.h"
#include "online_rf.h"
#include "linear_larank.h"
#include "online_mcboost.h"
#include "online_mclpboost.h"

#if 0

int main() {
    // Create a socket (IPv4, TCP)
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Listen to port 9999 on any address
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(9999);    // htons is necessary to convert a number to
                                        // network byte order
    if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Start listening. Hold at most 1 connection in the queue
    if (listen(sockfd, 1) < 0) {
        std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Grab a connection from the queue
    auto addrlen = sizeof(sockaddr);
    int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
    if (connection < 0) {
        std::cout << "Failed to grab connection. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Number of classes and features.
    const int numClasses = 2;
    const int numFeaturesPhotodiode = 2;
    const int numFeaturesEuler = 3;
    const int numFeaturesQuaternions = 4;

    // Load the hyperparameters
    Hyperparameters hp("conf/omcb.conf");

    // Models with Photodiodes as feature inputs.
    Classifier* pModelOnlineTreePhotodiode = NULL;
    Classifier* pModelOnlineRFPhotodiode = NULL;
    Classifier* pModelMCLPBoostPhotodiode = NULL;
    Classifier* pModelLinearLaRankPhotodiode = NULL;

    // Models with Euler angles as feature inputs.
    Classifier* pModelOnlineTreeEuler = NULL;
    Classifier* pModelOnlineRFEuler = NULL;
    Classifier* pModelMCLPBoostEuler = NULL;
    Classifier* pModelLinearLaRankEuler = NULL;

    // Models with Quaternions as feature inputs.
    Classifier* pModelOnlineTreeQuaternions = NULL;
    Classifier* pModelOnlineRFQuaternions = NULL;
    Classifier* pModelMCLPBoostQuaternions = NULL;
    Classifier* pModelLinearLaRankQuaternions = NULL;


    // New model command.
    const char* initCmd = "init";
    const int initCmdLen = 4;

    // Save model command.
    const char* saveCmd = "save";
    const int saveCmdLen = 4;

    // Load model command.
    const char* loadCmd = "load";
    const int loadCmdLen = 4;

    // Exit command.
    const char* exitCmd = "exit";
    const int exitCmdLen = 4;

    const char* trainCmd = "train";
    const int trainCmdLen = 5;

    // Read from the connection
    while(1)
    {
        char buffer[100];
        const int bufferLen = 100;
        auto bytesRead = read(connection, buffer, bufferLen);
        std::cout << "Received command: " << buffer;

        if (std::equal(initCmd, initCmd+initCmdLen, buffer))
        {
#if 0
            // Models with Photodiodes as feature inputs.
            pModelOnlineTreePhotodiode = new OnlineTree(hp, numClasses, numFeaturesPhotodiode,
                dataset_tr.m_minFeatRange, dataset_tr.m_maxFeatRange);
        
            pModelOnlineRFPhotodiode = new OnlineRF(hp, numClasses, numFeaturesPhotodiode,
                dataset_tr.m_minFeatRange, dataset_tr.m_maxFeatRange);

            pModelMCLPBoostPhotodiode = new OnlineMCLPBoost(hp, numClasses, numFeaturesPhotodiode,
                dataset_tr.m_minFeatRange, dataset_tr.m_maxFeatRange);
            
            pModelLinearLaRankPhotodiode = new LinearLaRank(hp, numClasses, numFeaturesPhotodiode,
                dataset_tr.m_minFeatRange, dataset_tr.m_maxFeatRange);

            // Models with Euler angles as feature inputs.
            pModelOnlineTreeEuler = new OnlineTree(hp, numClasses, numFeaturesEuler,
                dataset_tr.m_minFeatRange, dataset_tr.m_maxFeatRange);
        
            pModelOnlineRFEuler = new OnlineRF(hp, numClasses, numFeaturesEuler,
                dataset_tr.m_minFeatRange, dataset_tr.m_maxFeatRange);

            pModelMCLPBoostEuler = new OnlineMCLPBoost(hp, numClasses, numFeaturesEuler,
                dataset_tr.m_minFeatRange, dataset_tr.m_maxFeatRange);
            
            pModelLinearLaRankEuler = new LinearLaRank(hp, numClasses, numFeaturesEuler,
                dataset_tr.m_minFeatRange, dataset_tr.m_maxFeatRange);

            // Models with Quaternions as feature inputs.
            pModelOnlineTreeQuaternions = new OnlineTree(hp, numClasses, numFeaturesQuaternions,
                dataset_tr.m_minFeatRange, dataset_tr.m_maxFeatRange);
        
            pModelOnlineRFQuaternions = new OnlineRF(hp, numClasses, numFeaturesQuaternions,
                dataset_tr.m_minFeatRange, dataset_tr.m_maxFeatRange);

            pModelMCLPBoostQuaternions = new OnlineMCLPBoost(hp, numClasses, numFeaturesQuaternions,
                dataset_tr.m_minFeatRange, dataset_tr.m_maxFeatRange);
            
            pModelLinearLaRankQuaternions = new LinearLaRank(hp, numClasses, numFeaturesQuaternions,
                dataset_tr.m_minFeatRange, dataset_tr.m_maxFeatRange);
#endif
        }

        // Load saved models.
        else if (std::equal(loadCmd, loadCmd+loadCmdLen, buffer))
        {
            // Deserialize.
            // Models with Photodiodes as feature inputs.
            pModelOnlineTreePhotodiode = NULL;
            pModelOnlineRFPhotodiode = NULL;
            pModelMCLPBoostPhotodiode = NULL;
            pModelLinearLaRankPhotodiode = NULL;

            // Models with Euler angles as feature inputs.
            pModelOnlineTreeEuler = NULL;
            pModelOnlineRFEuler = NULL;
            pModelMCLPBoostEuler = NULL;
            pModelLinearLaRankEuler = NULL;

            // Models with Quaternions as feature inputs.
            pModelOnlineTreeQuaternions = NULL;
            pModelOnlineRFQuaternions = NULL;
            pModelMCLPBoostQuaternions = NULL;
            pModelLinearLaRankQuaternions = NULL;

            std::string response = "TODO: implement load models (object deserialization)\n";
            send(connection, response.c_str(), response.size(), 0);
            break;
        }

        // Save models.
        else if (std::equal(saveCmd, saveCmd+saveCmdLen, buffer))
        {
            std::string response = "TODO: implement save models (object serialization)\n";
            send(connection, response.c_str(), response.size(), 0);
            break;
        }
            
        // Exit server loop.
        else if (std::equal(exitCmd, exitCmd+exitCmdLen, buffer))
        {
            std::string response = "Goodbye\n";
            send(connection, response.c_str(), response.size(), 0);
            break;
        }

        else if (std::equal(trainCmd, trainCmd+trainCmdLen, buffer))
        {
            /**
             * Expected command string format:
             *      train <pd:float> <euler1:float> <euler2:float> <euler3:float> <q1:float> <q2:float> <q3:float> <q4:float> <label:int>
             * 
             * e.g.: train 1.23 10.1 20.2 30.3 1.0001 1.0002 -1.0003 -1.0004 1
             **/

            try 
            {
                std::string cmdString(buffer, bufferLen);
                std::istringstream cmdStringStream(cmdString);

                vector<std::string> cmdTokens{istream_iterator<std::string>{cmdStringStream},
                        istream_iterator<std::string>{}};

                // Parse input string/ to floats.
                float pd = std::stof(cmdTokens[1]);
                float euler1 = std::stof(cmdTokens[2]);
                float euler2 = std::stof(cmdTokens[3]);
                float euler3 = std::stof(cmdTokens[4]);
                float q1 = std::stof(cmdTokens[5]);
                float q2 = std::stof(cmdTokens[6]);
                float q3 = std::stof(cmdTokens[7]);
                float q4 = std::stof(cmdTokens[8]);
                int label = std::stoi(cmdTokens[9]);

#if 0
                // TODO: Create sample object.
                Sample sample;

                // Train models with Photodiodes as feature inputs.
                pModelOnlineTreePhotodiode->update(sample);
                pModelOnlineRFPhotodiode->update(sample);
                pModelMCLPBoostPhotodiode->update(sample);
                pModelLinearLaRankPhotodiode->update(sample);

                // Models with Euler angles as feature inputs.
                pModelOnlineTreeEuler->update(sample);
                pModelOnlineRFEuler->update(sample);
                pModelMCLPBoostEuler->update(sample);
                pModelLinearLaRankEuler->update(sample);

                // Models with Quaternions as feature inputs.
                pModelOnlineTreeQuaternions->update(sample);
                pModelOnlineRFQuaternions->update(sample);
                pModelMCLPBoostQuaternions->update(sample);
                pModelLinearLaRankQuaternions->update(sample);
#endif
                std::string response = "Training\n";
                send(connection, response.c_str(), response.size(), 0);
            }
            catch (const std::exception& e)
            {
                // TODO: Log error in log file.
                std::string response = "Error\n";
                send(connection, response.c_str(), response.size(), 0);
            }
        }
        else
        {
            std::string response = "Invalid command\n";
            send(connection, response.c_str(), response.size(), 0);
        }

        // 
    }

    // Close the connections
    close(connection);
    close(sockfd);
}
#endif